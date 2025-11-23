#include "Utils.h"
#include <cctype>
#include <fstream>
#include <sstream>
#include <iostream>

namespace CCrypto{

namespace Utils{

    std::string loadTextFromFile(const std::string& fileName) {
        std::cout << "loadTextFromFile";
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


    // From the string passed, compute the frequency map
    std::vector<double> loadFrequencyCharFromText(const std::string& fileName){

        std::cout << "loadFrequencyCharFromText" << std::endl;

        std::string text = loadTextFromFile(fileName);

        std::vector<double> freqVector(256,0);

        // Computing the frequency without normalization
        for(unsigned char c: text){
            freqVector[c] += 1;
        }


        // normalize the only printable character
        int textLen = text.length();
        for(int i = 0; i < 256; i++){
            if(std::isprint(static_cast<char>(i))){
                freqVector[i] = freqVector[i] / textLen;
                std::cout << static_cast<char>(i) << ": ";
                std::cout << freqVector[i] << std::endl;
            }
        }

        return freqVector; // RVO return, no copy or move costructor
    }



}

}


