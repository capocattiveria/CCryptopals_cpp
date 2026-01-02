#include "xor.h"
#include <iostream>
#include "StringConverter.h"

namespace CCrypto::Xor {

/**==============================================================================================**/
std::string fixedXor2Char(const std::string& text, char char2Xor){

    std::string result;

    // for every char we xor against charToXor
    for(int i = 0; i < text.length(); i++){
        result += text[i] ^ char2Xor;
    }

    std::cout << "charXored: " << result << std::endl;

    return result;

}

/**==============================================================================================**/

std::vector<uint8_t> fixedXor(const std::vector<uint8_t>& bytes, uint8_t byte2Xor){

    std::vector<uint8_t> result;

    // for every char we xor against charToXor
    for(size_t i = 0; i < bytes.size(); i++){
        result.push_back( bytes[i] ^ byte2Xor );
    }

    return result;

}

/**==============================================================================================**/

std::vector<uint8_t> repeatingKeyXor(const std::vector<uint8_t>& bytes, const std::string& string2Xor ){

    std::vector<uint8_t> result;

    int length = string2Xor.length();

    // Init mod to cycle throgh the string2Xor
    int mod = 0;

    for( auto& byte: bytes ){
        result.push_back(byte ^ (uint8_t)string2Xor[mod]);

        // reset the counter when mod go over the string2Xor
        mod++;
        if(mod > length)
            mod=0;
    }
    
    return result;
}

}


