#include <iostream>
#include "S01C01.h"
using namespace CCrypto;

std::string myHexString = "49276d206b696c6c696e6720796f757220627261696e206c696b65206120706f69736f6e6f7573206d757368726f6f6d";

int s01c01() {

  std::string base64String = hex2base64(myHexString);

  std::cout<< "My hexString is: \n" << myHexString << std::endl;

  std::cout<< "My base64 string is: \n" << base64String << std::endl;

  return 0;

}
