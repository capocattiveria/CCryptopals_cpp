#include "S02C11.h"
#include "SslWrapper.h"
#include "Utils.h"
#include <cstdint>
#include <cstdlib>
#include <iostream>

#include <ctime>

/*
Now that you have ECB and CBC working:

[X] Write a function to generate a random AES key; that's just 16 random bytes.

Write a function that encrypts data under an unknown key --- that is, a function that generates
a random key and encrypts under it.

The function should look like:

encryption_oracle(your-input)
=> [MEANINGLESS JIBBER JABBER]
Under the hood, have the function append 5-10 bytes (count chosen randomly) before the 
plaintext and 5-10 bytes after the plaintext.

Now, have the function choose to encrypt under ECB 1/2 the time, and under CBC
the other half (just use random IVs each time for CBC). Use rand(2) to decide which to use.

Detect the block cipher mode the function is using each time. You should end up with a piece of
code that, pointed at a block box that might be encrypting ECB or CBC, tells you which one
is happening.
*/


size_t randomlen()
{
	return 5 + std::rand() % 6; // 5..10
}

std::vector<uint8_t> encryption_oracle(const std::vector<uint8_t>& inputBytes, bool& usedEcb)
{
	std::vector<uint8_t> encryptedBytes;
	
	std::vector<uint8_t> randomKey = CCrypto::SslWrapper::randomBytes(16);

	bool toEncryptWithEcb = false;

	/*
	* Under the hood, have the function append 5-10 bytes (count chosen randomly) before the 
	* plaintext and 5-10 bytes after the plaintext.
	*/
	std::vector<uint8_t> prefix = CCrypto::SslWrapper::randomBytes(randomlen());
	std::vector<uint8_t> suffix = CCrypto::SslWrapper::randomBytes(randomlen());

	std::vector<uint8_t> data;

	data.reserve(prefix.size() + inputBytes.size() + suffix.size());
	data.insert(data.end(), prefix.begin(), prefix.end());
	data.insert(data.end(), inputBytes.begin(), inputBytes.end());
	data.insert(data.end(), suffix.begin(), suffix.end());


	// choose if encrypt with ecb or cbc from a random byte
	toEncryptWithEcb = std::rand() % 2;
	usedEcb = toEncryptWithEcb;

	if(toEncryptWithEcb)
	{
		std::cout << "Using ECB" << std::endl;
		encryptedBytes = CCrypto::SslWrapper::aes128EncryptECB(data, randomKey, true);

	}
	else
	{
		std::cout << "Using CBC" << std::endl;
		std::vector<uint8_t> IV = CCrypto::SslWrapper::randomBytes(16);
		encryptedBytes = CCrypto::SslWrapper::aes128EncryptCBC(data, randomKey, IV, true);
	}

	return encryptedBytes;
}


void s02c11()
{

	std::vector<uint8_t> inputBytes(256,'A');

	std::srand(static_cast<unsigned>(std::time(nullptr)));

	constexpr int rounds = 20;
	int correct = 0;

	for (int i = 0; i < rounds; ++i)
	{
		bool usedEcb = false;
		std::vector<uint8_t> cipher = encryption_oracle(inputBytes, usedEcb);

		// ECB: identical plaintext blocks -> identical ciphertext blocks
		bool detectedEcb = CCrypto::Utils::countDuplicateBlocks(cipher) > 0;

		std::cout << "Detected " << (detectedEcb ? "ECB" : "CBC")
		          << (detectedEcb == usedEcb ? " [OK]" : " [WRONG]") << std::endl;

		if (detectedEcb == usedEcb)
			++correct;
	}

	std::cout << correct << "/" << rounds << " correct" << std::endl;
}


