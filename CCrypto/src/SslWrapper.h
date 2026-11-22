#pragma once

#include <cstdint>
#include <vector>
#include <openssl/evp.h>
#include <stdexcept>

namespace CCrypto{
namespace SslWrapper{

inline constexpr size_t AES_BLOCK=16;

/*
* @brief AES-128 in ECB mode, encryption.
* @param plaintext data to encrypt. With padding == false it must be a multiple of 16 bytes
* @param key       exactly 16 bytes
* @param padding   if true OpenSSL appends PKCS#7 padding
* @throws std::invalid_argument bad key length
* @throws std::runtime_error    OpenSSL failure (e.g. input not multiple of 16 without padding)
*/
std::vector<uint8_t> aes128EncryptECB( const std::vector<uint8_t>& plaintext,
									   const std::vector<uint8_t>& key ,
									   bool padding = false
									 );

/*
* @brief AES-128 in ECB mode, decryption.
* @param ciphertext data to decrypt, must be a multiple of 16 bytes
* @param key        exactly 16 bytes
* @param padding    if true OpenSSL checks and removes PKCS#7 padding.
*                   Keep it false when decrypting single blocks (e.g. inside CBC).
* @throws std::invalid_argument bad key length
* @throws std::runtime_error    OpenSSL failure (bad length or bad padding)
*/
std::vector<uint8_t> aes128DecryptECB( const std::vector<uint8_t>& ciphertext,
									   const std::vector<uint8_t>& key ,
									   bool padding = false
									 );


/*
* @brief AES-128 in CBC mode, encryption (built on top of the ECB primitive).
* @param plaintext data to encrypt. With padding == false it must be a multiple of 16 bytes
* @param key       exactly 16 bytes
* @param iv        exactly 16 bytes
* @param padding   if true PKCS#7 padding is appended before encrypting
* @throws std::invalid_argument bad key/IV length or unaligned input without padding
* @throws std::runtime_error    OpenSSL failure
*/
std::vector<uint8_t> aes128EncryptCBC( const std::vector<uint8_t>& plaintext,
									   const std::vector<uint8_t>& key ,
									   const std::vector<uint8_t>& iv ,
									   bool padding = false
									 );

/*
* @brief AES-128 in CBC mode, decryption (built on top of the ECB primitive).
* @param ciphertext data to decrypt, must be a multiple of 16 bytes
* @param key        exactly 16 bytes
* @param iv         exactly 16 bytes
* @param padding    if true the PKCS#7 padding is validated and removed
* @throws std::invalid_argument bad key/IV length or ciphertext not a multiple of 16
* @throws std::runtime_error    OpenSSL failure or invalid PKCS#7 padding
*/
std::vector<uint8_t> aes128DecryptCBC( const std::vector<uint8_t>& ciphertext,
									   const std::vector<uint8_t>& key ,
									   const std::vector<uint8_t>& iv ,
									   bool padding = false
									 );

/*
* @brief Cryptographically secure random bytes (OpenSSL RAND_bytes).
*        A random AES-128 key is randomBytes(AES_BLOCK).
* @param n number of bytes to generate
* @throws std::runtime_error OpenSSL failure
*/
std::vector<uint8_t> randomBytes(size_t n);


} // end namespace SslWrapper
} // end namespace CCrypto

