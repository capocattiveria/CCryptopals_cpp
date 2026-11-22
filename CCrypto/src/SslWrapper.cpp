#include "SslWrapper.h"

#include <climits>
#include <memory>
#include <string>

#include <openssl/err.h>
#include <openssl/rand.h>

namespace CCrypto::SslWrapper{

namespace {


// Builds an exception that includes the OpenSSL error queue message
std::runtime_error opensslError(const std::string& what)
{
	char buf[256] = {0};
	ERR_error_string_n(ERR_get_error(), buf, sizeof(buf));
	return std::runtime_error(what + ": " + buf);
}

// Common core: encryption and decryption differ only in the last argument of EVP_CipherInit_ex
std::vector<uint8_t> aes128ECB( const std::vector<uint8_t>& input,
								const std::vector<uint8_t>& key,
								bool padding,
								bool encrypt )
{
	if(key.size() != AES_BLOCK)
		throw std::invalid_argument("AES-128 wants a key of 16 bytes");

	if(input.size() > static_cast<size_t>(INT_MAX) - AES_BLOCK)
		throw std::invalid_argument("Input too large");

	// Fail early with a clear message when the input can't be processed as whole blocks.
	// Decrypting always needs whole blocks; encrypting needs them only without padding.
	if((!encrypt || !padding) && input.size() % AES_BLOCK != 0)
		throw std::invalid_argument("Input must be a multiple of 16 bytes");

	// The context is freed on every path, including exceptions
	std::unique_ptr<EVP_CIPHER_CTX, decltype(&EVP_CIPHER_CTX_free)>
		ctx(EVP_CIPHER_CTX_new(), EVP_CIPHER_CTX_free);
	if(!ctx)
		throw opensslError("EVP_CIPHER_CTX_new failed");

	if(EVP_CipherInit_ex(ctx.get(), EVP_aes_128_ecb(), nullptr, key.data(), nullptr, encrypt ? 1 : 0) != 1)
		throw opensslError("EVP_CipherInit_ex failed");

	if(EVP_CIPHER_CTX_set_padding(ctx.get(), padding ? 1 : 0) != 1)
		throw opensslError("EVP_CIPHER_CTX_set_padding failed");

	// The output can be up to one block longer than the input
	std::vector<uint8_t> out(input.size() + AES_BLOCK);
	int outl = 0;
	int finl = 0;

	if(EVP_CipherUpdate(ctx.get(), out.data(), &outl, input.data(), static_cast<int>(input.size())) != 1)
		throw opensslError("EVP_CipherUpdate failed");

	// Final writes AFTER the bytes already produced by Update
	if(EVP_CipherFinal_ex(ctx.get(), out.data() + outl, &finl) != 1)
		throw opensslError(encrypt ? "EVP_CipherFinal_ex failed" : "EVP_CipherFinal_ex failed (bad padding?)");

	out.resize(static_cast<size_t>(outl) + static_cast<size_t>(finl));
	return out;
}

// dst[i] ^= src[i] for one block, dst starts at dstOffset, src at srcOffset
void xorBlock( std::vector<uint8_t>& dst, size_t dstOffset,
			   const std::vector<uint8_t>& src, size_t srcOffset )
{
	for(size_t i = 0; i < AES_BLOCK; ++i)
		dst[dstOffset + i] ^= src[srcOffset + i];
}

void checkKeyAndIv( const std::vector<uint8_t>& key, const std::vector<uint8_t>& iv )
{
	if(key.size() != AES_BLOCK)
		throw std::invalid_argument("AES-128 wants a key of 16 bytes");
	if(iv.size() != AES_BLOCK)
		throw std::invalid_argument("CBC wants an IV of 16 bytes");
}

void addPkcs7( std::vector<uint8_t>& data )
{
	const size_t padLen = AES_BLOCK - (data.size() % AES_BLOCK); // 1..16, a full block if already aligned
	data.insert(data.end(), padLen, static_cast<uint8_t>(padLen));
}

void removePkcs7( std::vector<uint8_t>& data )
{
	if(data.empty())
		throw std::runtime_error("Invalid PKCS#7 padding: empty data");

	const uint8_t n = data.back();
	if(n == 0 || n > AES_BLOCK || n > data.size())
		throw std::runtime_error("Invalid PKCS#7 padding");

	for(size_t i = data.size() - n; i < data.size(); ++i)
		if(data[i] != n)
			throw std::runtime_error("Invalid PKCS#7 padding");

	data.resize(data.size() - n);
}

} // end anonymous namespace


/*************************************************************************************************
 * ECB  encrypt and decrypt
*************************************************************************************************/
std::vector<uint8_t> aes128EncryptECB( const std::vector<uint8_t>& plaintext,
									   const std::vector<uint8_t>& key,
									   bool padding )
{
	return aes128ECB(plaintext, key, padding, true);
}


std::vector<uint8_t> aes128DecryptECB( const std::vector<uint8_t>& ciphertext,
									   const std::vector<uint8_t>& key,
									   bool padding )
{
	return aes128ECB(ciphertext, key, padding, false);
}


/*************************************************************************************************
 * CBC  encrypt and decrypt using the ecb encrypting mode
*************************************************************************************************/

std::vector<uint8_t> aes128EncryptCBC( const std::vector<uint8_t>& plaintext,
									   const std::vector<uint8_t>& key ,
									   const std::vector<uint8_t>& iv ,
									   bool padding
									 )
{
	checkKeyAndIv(key, iv);

	std::vector<uint8_t> data = plaintext;
	if(padding)
		addPkcs7(data);
	else if(data.size() % AES_BLOCK != 0)
		throw std::invalid_argument("Input must be a multiple of 16 bytes");

	// C_i = E(P_i xor C_(i-1)), with C_0 = IV. Inherently sequential.
	std::vector<uint8_t> out;
	out.reserve(data.size());
	std::vector<uint8_t> prev = iv;

	for(size_t off = 0; off < data.size(); off += AES_BLOCK)
	{
		std::vector<uint8_t> block(data.begin() + off, data.begin() + off + AES_BLOCK);
		xorBlock(block, 0, prev, 0);
		prev = aes128ECB(block, key, false, true);
		out.insert(out.end(), prev.begin(), prev.end());
	}
	return out;
}

std::vector<uint8_t> aes128DecryptCBC( const std::vector<uint8_t>& ciphertext,
									   const std::vector<uint8_t>& key ,
									   const std::vector<uint8_t>& iv ,
									   bool padding
									 )
{
	checkKeyAndIv(key, iv);

	if(ciphertext.size() % AES_BLOCK != 0)
		throw std::invalid_argument("Ciphertext must be a multiple of 16 bytes");

	// P_i = D(C_i) xor C_(i-1), with C_0 = IV.
	// Unlike encryption, every D(C_i) is independent: decrypt everything in one ECB call, then xor.
	std::vector<uint8_t> out = aes128ECB(ciphertext, key, false, false);

	for(size_t off = 0; off < out.size(); off += AES_BLOCK)
	{
		if(off == 0)
			xorBlock(out, 0, iv, 0);
		else
			xorBlock(out, off, ciphertext, off - AES_BLOCK);
	}

	if(padding)
		removePkcs7(out);
	return out;
}



std::vector<uint8_t> randomBytes(size_t n)
{
	if(n > static_cast<size_t>(INT_MAX))
		throw std::invalid_argument("Too many random bytes requested");

	std::vector<uint8_t> out(n);
	if(n == 0)
		return out;

	if(RAND_bytes(out.data(), static_cast<int>(n)) != 1)
		throw opensslError("RAND_bytes failed");
	return out;
}



}// end CCrypto::SslWrapper
