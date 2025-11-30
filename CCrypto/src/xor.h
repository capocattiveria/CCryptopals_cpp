#ifndef XOR_H
#define XOR_H

#include <string>
#include <cstdint>
#include <vector>

namespace CCrypto::Xor {

    std::string fixedXor2Char(const std::string& text, char char2Xor);
    std::vector<uint8_t> fixedXor(const std::vector<uint8_t>& bytes, uint8_t byte2Xor);
}



#endif
