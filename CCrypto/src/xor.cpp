#include "xor.h"
#include <iostream>
#include "StringConverter.h"

namespace CCrypto::Xor {

std::string fixedXor2Char(const std::string& text, char char2Xor){

    std::string result;

    // for every char we xor against charToXor
    for(int i = 0; i < text.length(); i++){
        result += text[i] ^ char2Xor;
    }

    std::cout << "charXored: " << result << std::endl;

    return result;

}

std::vector<uint8_t> fixedXor(const std::vector<uint8_t>& bytes, uint8_t byte2Xor){

    std::vector<uint8_t> result;

    // for every char we xor against charToXor
    for(size_t i = 0; i < bytes.size(); i++){
        result.push_back( bytes[i] ^ byte2Xor );
    }

    //std::cout << "charXored: " << StringConverter::bytesToString(result)<< std::endl;

    return result;

}

}


