#include "SslWrapper.h"

#include <climits>
#include <memory>
#include <string>

#include <openssl/err.h>

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

} // end anonymous namespace


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

}// end CCrypto::SslWrapper
