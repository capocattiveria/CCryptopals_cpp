#include "S01C07.h"

#include <iostream>
#include <fstream>
#include <vector>
#include <stdexcept>
#include <openssl/evp.h>

#include "StringConverter.h"

using namespace CCrypto;

static std::string readFile() {
    const std::string fileName = "./assets/s01c07_base64_input.txt";
    std::ifstream file(fileName);
    if (!file)
        throw std::runtime_error("Impossible to open file: " + fileName);

    std::string result, line;
    while (std::getline(file, line))
        result += line;
    return result;
}

void s01c07() {
    const std::string base64Input = readFile();
    const std::string ciphertext = StringConverter::base64Decode(base64Input);

    const uint8_t key[] = "YELLOW SUBMARINE"; // 16 bytes + null terminator, EVP uses first 16

    std::vector<uint8_t> plaintext(ciphertext.size() + 16);
    int outLen = 0, finalLen = 0;

    EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
    EVP_DecryptInit_ex(ctx, EVP_aes_128_ecb(), nullptr, key, nullptr);
    EVP_DecryptUpdate(ctx, plaintext.data(), &outLen,
                      reinterpret_cast<const uint8_t*>(ciphertext.data()),
                      static_cast<int>(ciphertext.size()));
    EVP_DecryptFinal_ex(ctx, plaintext.data() + outLen, &finalLen);
    EVP_CIPHER_CTX_free(ctx);

    plaintext.resize(outLen + finalLen);
    std::cout << StringConverter::bytesToString(plaintext) << std::endl;
}
