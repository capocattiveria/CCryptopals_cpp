#include "Utils.h"
#include <cctype>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <format>
#include "StringConverter.h"
#include "xor.h"

namespace CCrypto::Utils{ 


/**==============================================================================================**/

std::string loadTextFromFile(const std::string& fileName) {

    std::ifstream file(fileName);

    if(!file.is_open()){
        return "";
    }
 
    if (!file) {
        throw std::runtime_error("Impossible to open file: " + fileName);
    }

    std::stringstream buffer;

    buffer << file.rdbuf();  // read entire file
    return buffer.str();
}


/**==============================================================================================**/

std::vector<double> getFrequencyCharInVector(const std::string& fileName){

    std::string text = loadTextFromFile(fileName);

    std::vector<double> frequencyVector(256,0);

    // Computing the frequency without normalization
    for(unsigned char c: text){
        if( c >= 65 && c <= 90 ){ // Uppercase [A-Z]
            // Adding the frequency both to [A-Z] and [a-z]
            frequencyVector[c] += 1;
            frequencyVector[c + 32] += 1;
        } 
        else if(c >= 97 && c <= 122 ){ // Lowercase [a-z]
            // Adding the frequency both to [a-z] and [a-z]
            frequencyVector[c] += 1;
            frequencyVector[c - 32] += 1;
        }
        else
            frequencyVector[c] += 1;
    }


    int textLen = text.length();
    for(int i = 0; i < 256; i++){
        
        // Only calculate and print if the character count is non-zero AND printable
        if(frequencyVector[i] > 0 && std::isprint(static_cast<char>(i))){
            // Normalization
            frequencyVector[i] = frequencyVector[i] / textLen;
        }

    }

    return frequencyVector; // RVO return, no copy or move costructor
}



/**==============================================================================================**/

double englishScore(const std::string& text, const std::vector<double>& frequencyVector ){

    double score = 0.0;
    int textLength = text.size();

    if(textLength == 0 ) return score;

    for ( uint16_t c : text) {
        if (c < 255 && std::isprint(c)) {
            score += frequencyVector[c];
        }
    }

    return score / textLength;
}

/**==============================================================================================**/

uint8_t getFixedXorFromBytesString(const std::string& bytesString, std::string& resultString, double& bestScore ) {
  
  // getting frequency char
  std::vector<double> freqVector = Utils::getFrequencyCharInVector("./assets/englishSample.txt");

  // convert baseTest in string
  std::vector<uint8_t> text = StringConverter::hexStringToBytes(bytesString);


  // Init the results
  uint8_t bestChar = 0x00;
  bestScore = -5000000;

  for(uint16_t i = 0; i < 256; i++ ){

    uint8_t key = (uint8_t)i;

    // xor against char
    std::vector<uint8_t> xoredVector = Xor::fixedXor(text, key);

    std::string xoredString = StringConverter::bytesToString(xoredVector);

    double newScore = Utils::englishScore(xoredString, freqVector);
    //std::cout << (unsigned int)i << ": " << newScore << std::endl;

    if( newScore >= bestScore ) {
      bestChar = (char)i;
      bestScore = newScore;
    }
  }

  // std::cout << "Best bytes: " << bestChar << std::endl;
  resultString = StringConverter::bytesToString(Xor::fixedXor(text, bestChar));

  return (uint8_t)bestChar;

}

/**==============================================================================================**/

uint8_t getFixedXorFromBytes(const 	std::vector<uint8_t>& bytes,
								std::string& resultString,
						  		const 	std::string& englishScorePath)
{
	// getting frequency char
	std::vector<double> freqVector = Utils::getFrequencyCharInVector(englishScorePath);

	 // Init the results
  	uint8_t bestChar = 0x00;
	double bestScore = -5000000;



  	for(uint16_t i = 0; i < 256; i++ ){

		uint8_t key = (uint8_t)i;

		// xor against char
		std::vector<uint8_t> xoredVector = Xor::fixedXor(bytes, key);

		std::string xoredString = StringConverter::bytesToString(xoredVector);

		double newScore = Utils::englishScore(xoredString, freqVector);
		//std::cout << (unsigned int)i << ": " << newScore << std::endl;

		if( newScore >= bestScore ) {
		  bestChar = (char)i;
		  bestScore = newScore;
		}
	}

	//StringConverter::bytesToString(Xor::fixedXor(text, bestChar));

  return (uint8_t)bestChar;

}

/**==============================================================================================**/

void hexDump(const std::vector<uint8_t>& bytes){

    for (size_t i = 0; i < bytes.size(); ++i) {
        if (i % 16 == 0) 
            std::cout << std::format("\n{:04x}: ", i);

        std::cout << std::format("{:02x} ", bytes[i]);
    }
    std::cout << std::endl;
}


/**==============================================================================================**/
int popcount(uint8_t byte) {

    int count = 0;

    while(byte > 0){

        if( byte & 1 ) count++;

        // Note: Shifting introduce a zero on the most significant bit,
        // because the byte is unsigned int.
        // It would have been different if was a signed int.
        byte>>=1;
    }

    return count;
}


/**==============================================================================================**/
int hammingDist(const std::vector<uint8_t>& b0, const std::vector<uint8_t>& b1){

    // edge case
    if(b0.size() != b1.size()) return -1;

    int count = 0;
    for(size_t i = 0; i < b0.size(); i++ ){
        count += popcount(b0[i] ^ b1[i]);
    }
    return count;

}


}// End namespace CCrypto::Utils


