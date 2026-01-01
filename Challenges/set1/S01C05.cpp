#include "S01C05.h"

#include <iostream>
#include "Utils.h"
#include "StringConverter.h"
#include "xor.h"

using namespace CCrypto;


const std::string string2Xor = "ICE";

/*
 * 0b3637272a2b2e63622c2e69692a23693a2a3c6324202d623d63343c2a26226324272765272
 * a282b2f20430a652e2c652a3124333a653e2b2027630c692b20283165286326302e27282f
*/

std::string string2Encrypt = "Burning 'em, if you ain't quick and nimble \
I go crazy when I hear a cymbal";

// Repeating xor through ICE word
void s01c05(){

  std::cout<<" ****** Set 1 Challenge 5 ******" << std::endl;

  std::vector<unsigned char> bytes = StringConverter::stringToBytes(string2Encrypt);

  std::vector<uint8_t> encryptedBytes = Xor::repeatingKeyXor(bytes, string2Xor);

  // Dump the bytes encrypted on standard output
  Utils::hexDump(encryptedBytes);

}





