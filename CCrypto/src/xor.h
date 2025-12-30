#ifndef XOR_H
#define XOR_H

#include <string>
#include <cstdint>
#include <vector>

namespace CCrypto::Xor {

    std::string fixedXor2Char(const std::string& text, char char2Xor);

    std::vector<uint8_t> fixedXor(const std::vector<uint8_t>& bytes, uint8_t byte2Xor);


    /* @brief In repeating-key XOR, you'll sequentially apply each byte of the key; the first byte of 
     * plaintext will be XOR'd against I, the next C, the next E, 
     * then I again for the 4th byte, and so on
    */
    std::vector<uint8_t> repeatingKeyXor(const std::vector<uint8_t>& bytes, const std::string& string2Xor );

}



#endif
