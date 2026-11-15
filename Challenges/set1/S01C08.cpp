#include "S01C08.h"

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <unordered_set>
#include <stdexcept>

#include "StringConverter.h"

using namespace CCrypto;

static std::vector<std::string> readLines() {
    const std::string fileName = "./assets/s01c08_base64_input.txt";
    std::ifstream file(fileName);
    if (!file)
        throw std::runtime_error("Impossible to open file: " + fileName);

    std::vector<std::string> lines;
    std::string line;
    while (std::getline(file, line))
        if (!line.empty())
            lines.push_back(line);
    return lines;
}

static size_t countDuplicateBlocks(const std::vector<unsigned char>& bytes) {
    constexpr size_t blockSize = 16;
    std::unordered_set<std::string> seenBlocks;
    size_t duplicates = 0;

    for (size_t i = 0; i + blockSize <= bytes.size(); i += blockSize) {
        std::string block(bytes.begin() + i, bytes.begin() + i + blockSize);
        if (!seenBlocks.insert(block).second)
            ++duplicates;
    }
    return duplicates;
}

void s01c08() {
    const std::vector<std::string> lines = readLines();

    size_t bestLineIndex = 0;
    size_t bestDuplicateCount = 0;

    for (size_t i = 0; i < lines.size(); ++i) {
        const std::vector<unsigned char> bytes = StringConverter::hexStringToBytes(lines[i]);
        const size_t duplicates = countDuplicateBlocks(bytes);
        if (duplicates > bestDuplicateCount) {
            bestDuplicateCount = duplicates;
            bestLineIndex = i;
        }
    }

    if (bestDuplicateCount == 0) {
        std::cout << "No ECB-encrypted ciphertext found\n";
        return;
    }

    std::cout << "Line " << bestLineIndex << " is likely AES-128-ECB encrypted ("
              << bestDuplicateCount << " duplicate blocks)\n";
    std::cout << lines[bestLineIndex] << std::endl;
}


