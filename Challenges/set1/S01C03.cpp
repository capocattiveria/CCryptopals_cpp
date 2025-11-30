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

/**
 * @brief This function takes two parameters.
 *
 * @param bytesString  The hex string to analyze. The function attempts to
 *                     recover the original English text by XOR-ing it with
 *                     all possible characters and evaluating the results.
 *
 * @param resultString The output string that will contain the most likely
 *                     English plaintext.
 */
uint8_t fixedXorFromBytesString(const std::string& bytesString, std::string& resultString) {
  
  // getting frequency char
  std::vector<double> freqVector = Utils::getFrequencyCharInVector("./assets/englishSample.txt");

  // convert baseTest in string
  std::vector<uint8_t> text = StringConverter::hex2Bytes(bytesString);


  // Init the results
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
  resultString = StringConverter::bytesToString(Xor::fixedXor(text, bestChar));

  return (uint8_t)bestChar;

}


/** Function that execute the base test of Cryptopals **/
void s01c03(){

  std::string resultString;

  uint8_t resultByte = fixedXorFromBytesString(baseTest, resultString);

  std::cout << "Best result text: " << resultString << '\n'
          << "XOR key: 0x" << std::hex << static_cast<int>(resultByte) << std::dec
          << " (" << static_cast<int>(resultByte) << ")\n";

  return;

}
