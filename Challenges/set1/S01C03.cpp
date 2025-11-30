#include "S01C03.h"
#include <string>
#include <iostream>
#include <sys/types.h>
#include <vector>
#include <cstdint>

#include "Utils.h"
#include "StringConverter.h"
#include "xor.h"

using namespace CCrypto;

const std::string baseTest = "1b37373331363f78151b7f2b783431333d78397828372d363c78373e783a393b3736";


void s01c03(){

  // getting frequency char
  std::vector<double> freqVector = Utils::getFrequencyCharInVector("./CCrypto/assets/englishSample.txt");

  // convert baseTest in string
  std::vector<uint8_t> text = StringConverter::hex2Bytes(baseTest);

  //std::cout << "text: " << text << std::endl;

  uint8_t bestChar = 0x00;
  double bestScore = -5000000;

  for(uint16_t i = 0; i < 256; i++ ){

    uint8_t key = (uint8_t)i;

    // xor against char
    std::vector<uint8_t> xoredVector = Xor::fixedXor(text, key);

    std::string xoredString = StringConverter::bytesToString(xoredVector);

    double newScore = Utils::englishScore(xoredString, freqVector);
    //std::cout << (unsigned int)i << ": " << newScore << std::endl;

    if( newScore >= bestScore ) {
      bestChar = (char)i;
      bestScore = newScore;
    }
  }

  // std::cout << "Best bytes: " << bestChar << std::endl;
  std::cout << "Best result: " << StringConverter::bytesToString(Xor::fixedXor(text, bestChar)) << std::endl;
  std::cout << "Best score: " << bestScore << std::endl;

  return;

}

