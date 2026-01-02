#include "S01C06.h"

#include <iostream>
#include <fstream>

//CCrypto header
#include "Utils.h"
#include "StringConverter.h"

const std::string s1 = "this is a test";
const std::string s2 = "wokka wokka!!!";


std::vector<uint8_t> readInput(){
  
  std::string fileName = "./assets/s01c06_Input.txt";

  std::ifstream file(fileName);

  if (!file) {
        throw std::runtime_error("Impossible to open file: " + fileName);
  }
}

void s01c06(){

  std::cout << "Hamming distance: " << Utils::hammingDist(StringConverter::stringToBytes(s1), StringConverter::stringToBytes(s2));

  // Loop from 2 to 40 to find the more accurate KEYSIZE
  // size_t key = 2;
  //
  // for(int i = 2; i < 40; i++ ){
  //
  // }


}
