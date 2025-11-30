#include "S01C04.h"
#include "S01C03.h"
#include <iostream>
#include <fstream>
#include <string>
#include "xor.h"

#include "StringConverter.h"
#include "Utils.h"

using namespace CCrypto;


void s01c04(){


  double bestLineScore = -50000;
  std::string bestLine;
  std::string bestResultString;
  uint8_t bestBytes = 0x00;

  std::cout << "=> You are testing the Set 1 - Challenge 4" << std::endl;
  
  std::string fileName = "./assets/s01c04_test.txt";

  std::ifstream file(fileName);

  if (!file) {
        throw std::runtime_error("Impossible to open file: " + fileName);
  }

  // Iterating trough the lines of the file
  std::string line;
  while(std::getline(file, line)){

    std::string resultString;
    double currentLineScore;
    uint8_t currentBytes;

    currentBytes = fixedXorFromBytesString(line, resultString, currentLineScore);

    // update the score
    if(bestLineScore < currentLineScore){
      bestLineScore = currentLineScore;
      bestBytes = currentBytes;
      bestLine = line;
      bestResultString = resultString;
    }
  }

  std::cout << "Best line: " << bestLine << '\n'
            << "Best result text: " << bestResultString<< '\n'
            << "XOR key: 0x" << std::hex << static_cast<int>(bestBytes) << std::dec
            << " (" << static_cast<int>(bestBytes) << "-> " << static_cast<char>(bestBytes) << ")\n";


}

