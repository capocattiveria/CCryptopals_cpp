#include "s01c02.h"
#include <stdexcept>
#include <string>
#include "StringConverter.h"
#include <iostream>
#include <sstream>
#include <iomanip>


// result: 746865206b696420646f6e277420706c6179
//
std::string vectorToHexString(const std::vector<unsigned char>& bytes) {
    std::ostringstream ss;
    for (unsigned char byte : bytes) {
        ss << std::hex << std::setfill('0') << std::setw(2) << static_cast<int>(byte);
    }
    return ss.str();
}


void s01c02(){

  const std::string leftString = "1c0111001f010100061a024b53535009181c";
  const std::string rightString = "686974207468652062756c6c277320657965";

  // Check the length of the strings
  if(leftString.size() != rightString.size())
    throw std::invalid_argument("Left and right string have different sizes");


  // Getting the bytes array from hex
  std::vector<unsigned char> leftBytes  = CCrypto::StringConverter::hex2Bytes(leftString);
  std::vector<unsigned char> rightBytes = CCrypto::StringConverter::hex2Bytes(rightString);


  std::vector<unsigned char> xorBytes;

  std::string result;

  for(int i = 0; i < leftString.size(); i++){
    // Xor of the byte and then transform in String
    xorBytes.push_back(leftBytes[i] ^ rightBytes[i]);
    result += leftBytes[i] ^ rightBytes[i];

  }

  std::cout << "result: " << result << std::endl;

  std::cout << "result in Hex: " << vectorToHexString(xorBytes) << std::endl;



}


