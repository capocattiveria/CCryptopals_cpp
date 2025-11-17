#include "Utils.h"
#include <cctype>
#include <cstdint>

namespace CCrypto{

namespace Utils{


    // From the string passed, compute the frequency map
    std::vector<double> getFrequencyCharFromText(const std::string& str){

        std::vector<double> freqVector(256,0);

        // Init map with all the printable char
        // for( int i = 0  ; i < 256; i++ ){
        //     freqVector[] = 0;
        // }
        
        int strLen = str.length();

        // Computing the frequency without normalization
        for(unsigned char c: str){
            // check if the char is printable
            freqVector[c] = freqVector[c] + (1 / strLen);
        }

        return freqVector; // RVO return, no copy or move costructor
    }

/** [] Function for HexDump */

}

}


