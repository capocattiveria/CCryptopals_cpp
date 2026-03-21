#include "S01C06.h"

#include <iostream>
#include <fstream>

//CCrypto header
#include "Utils.h"
#include "StringConverter.h"

const std::string s1 = "this is a test";
const std::string s2 = "wokka wokka!!!";


// Read the base64File
std::string readFile(){

  std::string base64String;

  std::string fileName = "./assets/s01c06_base64_input.txt";

  std::ifstream file(fileName);

  // The file in input must exists.
  if (!file) {
        throw std::runtime_error("Impossible to open file: " + fileName);
  }

  // Iterating trough the lines of the file
  std::string line;
  while(std::getline(file, line)){
    base64String += line;
  }

  return base64String;

}

void s01c06(){

  // step 1, the hamming distance must be 37
  std::cout << "Hamming distance: " << Utils::hammingDist(StringConverter::stringToBytes(s1), StringConverter::stringToBytes(s2));

  // Put the file input into a string
  std::string base64string = readFile();

  //std::cout << base64string;

  // Loop from 2 to 40 to find the more accurate KEYSIZE
  size_t key = 2;
  for(int i = 2; i < 40; i++ ){
    
  }


}
