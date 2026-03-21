#include <iostream>
#include <sstream>
#include <stdexcept>
#include <sstream>
#include <iomanip>
#include <cstdint>
#include <array>
#include <string_view>

#include "StringConverter.h"

namespace CCrypto::StringConverter {

// constexpr function to generate table at compile time
constexpr std::array<int, 256> generate_b64_lookup() {
    std::array<int, 256> table{};
    // Inizializziamo con -1 (usando un valore intero)
    for (auto& cell : table) cell = -1;

    constexpr std::string_view chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz"\
                                       "0123456789+/";
    
    for (int i = 0; i < 64; ++i) {
        // Il cast a unsigned char è fondamentale per gestire l'indice dell'array
        table[static_cast<unsigned char>(chars[i])] = i;
    }
    
    return table;
}

// La tabella viene creata dal compilatore e inserita nel segmento dati dell'eseguibile
static constexpr std::array<int, 256> b64_lookup = generate_b64_lookup();

/**==============================================================================================**/

// Base64 definition
const std::string base64_chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz"\
                                 "0123456789+/";



/**==============================================================================================**/

// This function return the integer associated with the letter of an hex char
int hexCharToInt(char c){

    if (c >= '0' && c <= '9') return c - '0';
    if (c >= 'a' && c <= 'f') return c - 'a' + 10;
    if (c >= 'A' && c <= 'F') return c - 'A' + 10;
    
    return -1;
}

/**==============================================================================================**/

/** Return the array of bytes from a valid hex string  **/
std::vector<unsigned char> hexStringToBytes(const std::string& hexString)
{
    
    std::vector<unsigned char> bytes;

    // Hex string should have even length
    if( hexString.length() % 2 != 0 )
        throw std::invalid_argument("Hex string must have even length");

    // Loop through the string inspecting pair by pair
    for(size_t i = 0; i < hexString.length(); i += 2)
    {
        // Get the left and rigth value of the HexBytes
        int leftValue   = hexCharToInt(hexString.at(i));
        int rightValue  = hexCharToInt(hexString.at(i+1));

        // Check if the values are valid
        if( leftValue == -1 || rightValue == -1 )
            throw std::invalid_argument("Invalid hex character");

        bytes.push_back( leftValue << 4 | rightValue );
    }

    return bytes;
}

/**==============================================================================================**/

//encoding in basa64
std::string bytesToBase64Encode(const std::vector<unsigned char>& bytes)
{
    std::string encodedString;

    // Get 3 bytes in each cycle
    for(size_t i = 0; i < bytes.size(); i+=3){

        unsigned int value; 
        int padding = 0;

        // First bytes always present
        value = bytes[i] << 16;

        // Check if second bytes exists.
        if( i + 1 < bytes.size() )
            value |= bytes[i+1] << 8;	
        else
            padding ++;

        // Check if thirth bytes exists.
        if( i + 2 < bytes.size() )
            value |= bytes[i+2];
        else
            padding++;

        // Converts the 24 bits to base64 characters
        encodedString += base64_chars[(value >> 18) & 0x3F];
        encodedString += base64_chars[(value >> 12) & 0x3F];
        encodedString += (padding < 2) ?  base64_chars[(value >> 6) & 0x3F] : '=';
        encodedString += (padding < 1) ?  base64_chars[value & 0x3F] : '=';

    }

    return encodedString;
}

/**==============================================================================================**/
std::string hexToBase64Encode(const std::string& hexString)
{
    std::vector<unsigned char> bytes = hexStringToBytes(hexString);
    return bytesToBase64Encode(bytes);
}

/**==============================================================================================**/
/**
 * @brief Decodes a Base64-encoded string to raw bytes.
 * 
 * Converts Base64 ASCII representation back to binary data.
 * Processes 4 Base64 characters (24 bits) into 3 output bytes.
 * 
 * @param base64String Base64-encoded input (may contain newlines)
 * @return Decoded binary data as string
 * 
 * @example
 * @code
 * std::string data = base64Decode("SGVsbG8="); // Returns "Hello"
 * @endcode
 */
std::string base64Decode(const std::string &base64String) 
{

    std::string out;
    
    for(size_t i = 0; i + 3 < base64String.size(); i += 4) {
        // Salta padding e newline
        if (base64String[i] == '=' || base64String[i] == '\n') 
            break;
        
        // Converti da ASCII a valore Base64
        uint8_t c1 = b64_lookup[static_cast<uint8_t>(base64String[i])];
        uint8_t c2 = b64_lookup[static_cast<uint8_t>(base64String[i+1])];
        uint8_t c3 = b64_lookup[static_cast<uint8_t>(base64String[i+2])];
        uint8_t c4 = b64_lookup[static_cast<uint8_t>(base64String[i+3])];
        
        // Skip se caratteri invalidi
        if (c1 == 0xFF || c2 == 0xFF) continue;
        
        // Decodifica 4→3 bytes
        uint8_t v1 = (c1 << 2) | (c2 >> 4);
        uint8_t v2 = (c2 << 4) | (c3 >> 2);
        uint8_t v3 = (c3 << 6) | c4;
        
        out.push_back(v1);
        
        // Gestisci padding
        if (base64String[i+2] != '=') 
            out.push_back(v2);
        if (base64String[i+3] != '=') 
            out.push_back(v3);
    }
    

  return out;
}

/**==============================================================================================**/

std::string vectorToHexString(const std::vector<unsigned char>& bytes) {
    std::ostringstream ss;
    for (unsigned char byte : bytes) {
        ss << std::hex << std::setfill('0') << std::setw(2) << static_cast<int>(byte);
    }
    return ss.str();
}

/**==============================================================================================**/

std::string bytesToString(const std::vector<unsigned char>& bytes) {
    return std::string(bytes.begin(), bytes.end());
}

/**==============================================================================================**/

std::vector<uint8_t> stringToBytes(const std::string& str){
    return std::vector<uint8_t>(str.begin(),str.end());
}



}

