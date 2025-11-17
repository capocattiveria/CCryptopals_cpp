#include <sstream>
#include <stdexcept>
#include <sstream>
#include <iomanip>

#include "StringConverter.h"

namespace CCrypto {


        // Definizione base64
        const std::string base64_chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

        int hexChar2Int(char c){

            if (c >= '0' && c <= '9') return c - '0';
            if (c >= 'a' && c <= 'f') return c - 'a' + 10;
            if (c >= 'A' && c <= 'F') return c - 'A' + 10;
            
            return -1;
        }

        /** Return the array of bytes from a valid hex string  **/
        std::vector<unsigned char> hex2Bytes(const std::string& hexString)
        {
            
            std::vector<unsigned char> bytes;

            // Hex string should have even length
            if( hexString.length() % 2 != 0 )
                throw std::invalid_argument("Hex string must have even length");

            // Loop through the string inspecting pair by pair
            for(size_t i = 0; i < hexString.length(); i += 2)
            {
                // Get the left and rigth value of the HexBytes
                int leftValue   = hexChar2Int(hexString.at(i));
                int rightValue  = hexChar2Int(hexString.at(i+1));

                // Check if the values are valid
                if( leftValue == -1 || rightValue == -1 )
                    throw std::invalid_argument("Invalid hex character");

                bytes.push_back( leftValue << 4 | rightValue );
            }

            return bytes;
        }


        /** Convert vector of bytes to base64**/
        std::string bytes2Base64(const std::vector<unsigned char> bytes)
        {
            std::string result;

            for(size_t i = 0; i < bytes.size(); i+=3){

                // Get 3 bytes
                unsigned int value; 
                int padding = 0;

                // First bytes always present
                value = bytes[i] << 16;

                // Check if second bytes exists.
                if( i + 1 < bytes.size() )
                    value |= bytes[i+1] << 8;	
                else
                    padding ++;

                // Check if second bytes exists.
                if( i + 2 < bytes.size() )
                    value |= bytes[i+2];
                else
                    padding++;

                // Converts the 24 bits to base64 characters
                result += base64_chars[(value >> 18) & 0x3F];
                result += base64_chars[(value >> 12) & 0x3F];
                result += (padding < 2) ?  base64_chars[(value >> 6) & 0x3F] : '=';
                result += (padding < 1) ?  base64_chars[value & 0x3F] : '=';

            }

            return result;
        }


        std::string hex2base64(const std::string& hexString)
        {
            std::vector<unsigned char> bytes = hex2Bytes(hexString);
            return bytes2Base64(bytes);
        }

        std::string vectorToHexString(const std::vector<unsigned char>& bytes) {
            std::ostringstream ss;
            for (unsigned char byte : bytes) {
                // ss << std::hex << std::setfill('0') << std::setw(2) << static_cast<int>(byte);
                ss << std::hex << std::setw(2) << static_cast<int>(byte);
            }
            return ss.str();
        }

}

