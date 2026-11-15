#include "S02C09.h"

#include <cstdint>
#include <string>
#include <StringConverter.h>
#include <iostream>
#include <Utils.h>



void s02c09()
{

	std::string key = "YELLOW SUBMARINE";

	std::vector<uint8_t> bytesKey = CCrypto::StringConverter::stringToBytes(key);

	CCrypto::Utils::addPKCS7Padding(bytesKey, 20);
	
	CCrypto::Utils::hexDump(bytesKey);

	return;
}
