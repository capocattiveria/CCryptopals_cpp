#include <string>
#include "S01C03.h"
#include "Utils.h"
#include <iostream>
#include <filesystem>

using namespace CCrypto;

const std::string baseTest = "1b37373331363f78151b7f2b783431333d78397828372d363c78373e783a393b3736";

void s01c03(){

  Utils::loadFrequencyCharFromText("./CCrypto/assets/englishSample.txt");

  std::cout << std::filesystem::current_path() << std::endl;

  return;

}

