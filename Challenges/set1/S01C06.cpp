#include "S01C06.h"

#include <iostream>
#include <fstream>

//CCrypto header
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


int findKeySize(const std::string& encodedString) {
    double minNormalizedDistance = -1;
    int choosedKey = 0;
    
    for (int key = 2; key <= 40; key++) {
        // Verifica che ci siano abbastanza byte
        if (encodedString.size() < (size_t)(4 * key)) break;
        
        // Prendi 4 blocchi
        std::string block1 = encodedString.substr(0 * key, key);
        std::string block2 = encodedString.substr(1 * key, key);
        std::string block3 = encodedString.substr(2 * key, key);
        std::string block4 = encodedString.substr(3 * key, key);
        
        // Converti a vector
        auto v1 = std::vector<uint8_t>(block1.begin(), block1.end());
        auto v2 = std::vector<uint8_t>(block2.begin(), block2.end());
        auto v3 = std::vector<uint8_t>(block3.begin(), block3.end());
        auto v4 = std::vector<uint8_t>(block4.begin(), block4.end());
        
        // Calcola 6 distanze (tutte le coppie)
        double d1 = Utils::hammingDist(v1, v2);
        double d2 = Utils::hammingDist(v1, v3);
        double d3 = Utils::hammingDist(v1, v4);
        double d4 = Utils::hammingDist(v2, v3);
        double d5 = Utils::hammingDist(v2, v4);
        double d6 = Utils::hammingDist(v3, v4);
        
        // Media normalizzata
        double dist = (d1 + d2 + d3 + d4 + d5 + d6) / (6.0 * key);
        
        // Trova il minimo
        if (minNormalizedDistance == -1 || dist < minNormalizedDistance) {
            minNormalizedDistance = dist;
            choosedKey = key;
        }
        
        // std::cout << "Key " << key << ": " << dist << std::endl;
    }
    
   
    return choosedKey;
}

void s01c06(){

	// Step 1, the hamming distance must be 37
	std::cout << "Hamming distance: "
			<< Utils::hammingDist(StringConverter::stringToBytes(s1),
									StringConverter::stringToBytes(s2))
			<< std::endl;

	// test the decoding function
	// std::cout << "decode SVGs: " << StringConverter::base64Decode("SGVs") <<
	// std::endl;

	// Put the file input into a string
	std::string base64string = readFile();

	std::string encodedString = StringConverter::base64Decode(base64string);
	
	int choosedKey = findKeySize(encodedString);

	std::cout << "Best key size: " << choosedKey ;

    

	return;
}
