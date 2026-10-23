cmake_minimum_required(VERSION 3.20)
project(cryptopals CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(OpenSSL REQUIRED)

add_library(CCrypto STATIC
    CCrypto/src/StringConverter.cpp
    CCrypto/src/Utils.cpp
    CCrypto/src/xor.cpp
)
target_include_directories(CCrypto PUBLIC CCrypto/src)
target_compile_options(CCrypto PRIVATE -Wall -Wextra)

file(GLOB CHALLENGE_SOURCES CONFIGURE_DEPENDS Challenges/set*/*.cpp)
add_executable(cryptopals Challenges/main.cpp ${CHALLENGE_SOURCES})
target_include_directories(cryptopals PRIVATE Challenges)
target_compile_options(cryptopals PRIVATE -Wall -Wextra)
target_link_libraries(cryptopals PRIVATE CCrypto OpenSSL::Crypto)
