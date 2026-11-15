#include "S02C10.h"
#include "StringConverter.h"
#include "xor.h"
#include "SslWrapper.h"
#include <iostream>
#include <fstream>
#include <stdexcept>
#include <openssl/evp.h>
#include <openssl/err.h>


using namespace CCrypto;


/**************************************************************************************************
Implement CBC mode
CBC mode is a block cipher mode that allows us to encrypt irregularly-sized messages, despite the 
fact that a block cipher natively only transforms individual blocks.

In CBC mode, each ciphertext block is added to the next plaintext block before the next call to 
the cipher core.

The first plaintext block, which has no associated previous ciphertext block, is added to a "fake 
0th ciphertext block" called the initialization vector, or IV.

Implement CBC mode by hand by taking the ECB function you wrote earlier, making it encrypt instead 
of decrypt (verify this by decrypting whatever you encrypt to test), and using your XOR function 
from the previous exercise to combine them.

The file here is intelligible (somewhat) when CBC decrypted against "YELLOW SUBMARINE" with an IV
of all ASCII 0 (\x00\x00\x00 &c)

**************************************************************************************************/

static std::string readFile() {
    const std::string fileName = "./assets/s02c10_base64_input.txt";
    std::ifstream file(fileName);
    if (!file)
        throw std::runtime_error("Impossible to open file: " + fileName);

    std::string result, line;
    while (std::getline(file, line))
        result += line;
    return result;
}


constexpr size_t BLOCK= 16;

void s02c10()
{

    const std::string base64Input = readFile();
    const std::string cipherText = CCrypto::StringConverter::base64Decode(base64Input);

	std::vector<uint8_t> encryptedData(cipherText.begin(),cipherText.end());

    const uint8_t key[] = "YELLOW SUBMARINE"; // 16 bytes + null terminator, EVP uses first 16
	std::vector<uint8_t >vkey(std::begin(key), std::end(key) - 1);

	std::vector<uint8_t> IV( BLOCK ,0);
	std::vector<uint8_t> plainText;


	bool isLastBlock = false;
	auto it = encryptedData.begin();

	while(!isLastBlock)
	{
		size_t rimasti = std::distance(it, encryptedData.end());
		if(rimasti >= BLOCK)
		{
			auto blockInput = std::vector<uint8_t>(it, it + BLOCK );
			auto out = SslWrapper::aes128DecryptECB(blockInput, vkey);

			auto xoredPlainText = Xor::bytesXorBytes(out, IV);

			// Append del testo criptato
			plainText.insert(plainText.end(), xoredPlainText.begin(), xoredPlainText.end());

			// Update of the 16 byte of IV, the Next Iteration is the encryptedText that will be xored with the bytes
			IV = blockInput;
			it = it + BLOCK;

		}
		else
		{
			isLastBlock = true;
			// Se avanzano byte, il ciphertext non e' multiplo di 16
			if(rimasti != 0)
				throw std::invalid_argument("Ciphertext is not a multiple of 16 bytes");

			// Check del padding PKCS#7: l'ultimo byte n dice quanti byte di padding ci sono
			if(plainText.empty())
				throw std::runtime_error("Invalid PKCS#7 padding: empty data");

			const uint8_t n = plainText.back();
			if(n == 0 || n > BLOCK || n > plainText.size())
				throw std::runtime_error("Invalid PKCS#7 padding");

			// Gli ultimi n byte devono valere tutti n
			for(size_t i = plainText.size() - n; i < plainText.size(); ++i)
				if(plainText[i] != n)
					throw std::runtime_error("Invalid PKCS#7 padding");

			// Ridimensiono adesso per ottenere il testo originale
			plainText.resize(plainText.size() - n);

			std::cout << std::string(plainText.begin(), plainText.end()) << std::endl;

		}
	}
}


