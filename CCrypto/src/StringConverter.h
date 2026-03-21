#ifndef STRING_CONVERTER_H
#define STRING_CONVERTER_H

#include <vector>
#include <string>
#include <cstdint>


namespace CCrypto::StringConverter {


    // Return a hex char 2 int
    int hexCharToInt(char c);

    /** Return the array of bytes from a valid hex string  **/
    std::vector<unsigned char> hexStringToBytes(const std::string& hexString);

    /** Convert vector of bytes to base64**/
    std::string bytesToBase64(const std::vector<unsigned char>& bytes);

    // Convert hex string to base64
    std::string hexToBase64(const std::string& hexString);

    // Convert base64 to bytes
    std::string base64ToBytes(const std::string& base64String);

    // Convert the vector of bytes to HexString representation
    std::string vectorToHexString(const std::vector<unsigned char>& bytes);

    // Convert the vector of bytes in String
    std::string bytesToString(const std::vector<unsigned char>& bytes);

    // Convert a string in vector of bytes
    std::vector<uint8_t> stringToBytes(const std::string& str);


}


#endif
