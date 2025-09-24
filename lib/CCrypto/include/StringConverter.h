#ifndef STRING_CONVERTER_H
#define STRING_CONVERTER_H

#include <vector>
#include <string>

namespace CCrypto {

    /**@class StringConverter
     * Class used to convert string in different format.
    */
    class StringConverter {

	// Static string, used to access the char through index.
	static const std::string base64_chars;
	
	// Return the integer associated at the char
	static int hexChar2Int(char c){
	    if (c >= '0' && c <= '9') return c - '0';
	    if (c >= 'a' && c <= 'f') return c - 'a' + 10;
	    if (c >= 'A' && c <= 'F') return c - 'A' + 10;

	    return -1; // Invalid hex character
	}


    public: 

	/** Return the array of bytes from a valid hex string  **/
	static std::vector<unsigned char> hex2Bytes(const std::string& hexString);

	/** Convert vector of bytes to base64**/
	static std::string bytes2Base64(const std::vector<unsigned char> bytes);

	// Convert hex string to base64
	static std::string hex2base64(const std::string& hexString);

	// Convert the vector of bytes to HexString representation
	static std::string vectorToHexString(const std::vector<unsigned char>& bytes);

	
    };


};


#endif // STRING_CONVERT_H
