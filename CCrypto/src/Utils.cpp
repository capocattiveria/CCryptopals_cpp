#include "Utils.h"
#include <bit>
#include <cctype>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <format>

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

    int count =0;
    for(int i = 0; i < b0.size(); i++ ){
        count += popcount(b0[i] ^ b1[i]);
    }
    return count;

}


}// End namespace CCrypto::Utils


