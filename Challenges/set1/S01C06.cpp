#include "S01C06.h"
#include "S01C03.h"

#include <iostream>
#include <fstream>
#include <algorithm>

//CCrypto header
#include "xor.h"
#include "Utils.h"
#include "StringConverter.h"

const std::string s1 = "this is a test";
const std::string s2 = "wokka wokka!!!";


// Read the base64File
std::string readFile(){

	std::string base64String;

	std::string fileName = "./assets/s01c06_base64_input.txt";

	std::ifstream file(fileName);

  // The file in input must exists.
	if (!file) {
		throw std::runtime_error("Impossible to open file: " + fileName);
	}

	// Iterating trough the lines of the file
	std::string line;
	while(std::getline(file, line)){
		base64String += line;
	}

	return base64String;
}

/**
For each KEYSIZE, take the first KEYSIZE worth of bytes, and the second KEYSIZE worth of bytes,
and find the edit distance between them.
Normalize this result by dividing by KEYSIZE.
*/
int findKeySize(const std::string& encodedString) {
	double minNormalizedDistance = -1;
	int choosedKey = 0;

	for (int key = 2; key <= 40; key++) {

	    // Verify that there are enough bytes
	    if (encodedString.size() < (size_t)(4 * key)) break;

		// Getting 4 blocks of bytes
		std::string block1 = encodedString.substr(0 * key, key);
		std::string block2 = encodedString.substr(1 * key, key);
		std::string block3 = encodedString.substr(2 * key, key);
		std::string block4 = encodedString.substr(3 * key, key);

		// Convert the strings in buffers
		auto v1 = std::vector<uint8_t>(block1.begin(), block1.end());
		auto v2 = std::vector<uint8_t>(block2.begin(), block2.end());
		auto v3 = std::vector<uint8_t>(block3.begin(), block3.end());
		auto v4 = std::vector<uint8_t>(block4.begin(), block4.end());

		// Compute the distances
		double d1 = Utils::hammingDist(v1, v2);
		double d2 = Utils::hammingDist(v1, v3);
		double d3 = Utils::hammingDist(v1, v4);
		double d4 = Utils::hammingDist(v2, v3);
		double d5 = Utils::hammingDist(v2, v4);
		double d6 = Utils::hammingDist(v3, v4);

		// Normalizing the distances
		double dist = (d1 + d2 + d3 + d4 + d5 + d6) / (6.0 * key);

		// Choose the key if the distance is smaller
		if (minNormalizedDistance == -1 || dist < minNormalizedDistance) {
			minNormalizedDistance = dist;
			choosedKey = key;
		}
	}

	return choosedKey;
}

void s01c06(){

    // Step 1, the hamming distance must be 37
    std::cout << "Hamming distance: " << Utils::hammingDist(StringConverter::stringToBytes(s1),
											StringConverter::stringToBytes(s2))
									  << std::endl;

	// Put the file input into a string
	std::string base64string = readFile();

	std::string encodedString = StringConverter::base64Decode(base64string);
	int choosedKey = findKeySize(encodedString);

	std::cout << "Best key size: " << choosedKey << std::endl;

	// 5. break the ciphertext into blocks of KEYSIZE length
	// 6. Now transpose the blocks: make a block that is the first byte of every block,
	// and a block that is the second byte of every block, and so on
	std::vector<std::vector<uint8_t>> transposedBlock(choosedKey);
	
	for(size_t i = 0; i < encodedString.size(); i++)
	{
		int mod = i % choosedKey;
		transposedBlock[mod].push_back(encodedString[i]);
	}

	std::string key = "";

	for(size_t i = 0; i < choosedKey; i++)
	{
		std::string resultString;
		key += Utils::getFixedXorFromBytes(transposedBlock[i], resultString);
	}

	std::cout << key << std::endl;

	std::vector<unsigned char> cryptedBytes = StringConverter::stringToBytes(encodedString);

  	std::vector<uint8_t> decryptedBytes = Xor::repeatingKeyXor(cryptedBytes, key);

	std::cout << StringConverter::bytesToString(decryptedBytes) << std::endl;
	// std::cout << decryptedBytes << std::endl;



	return;
}
