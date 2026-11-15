# Minimum CMake version required
cmake_minimum_required(VERSION 3.20)

# Project name; CXX = C++ only (no C compiler needed)
project(cryptopals CXX)

# --- Global compiler settings ---
set(CMAKE_CXX_STANDARD 23)             # same as -std=c++23
set(CMAKE_CXX_STANDARD_REQUIRED ON)    # error if the compiler doesn't support it
set(CMAKE_CXX_EXTENSIONS OFF)          # -std=c++23 instead of -std=gnu++23
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)  # generate compile_commands.json for clangd

# Look for OpenSSL on the system (libssl-dev); fails if not found.
# Provides the OpenSSL::Crypto target, which corresponds to -lcrypto
find_package(OpenSSL REQUIRED)

# --- CCrypto library ---
# Static library (libCCrypto.a) built from these sources
add_library(CCrypto STATIC
    CCrypto/src/StringConverter.cpp
    CCrypto/src/Utils.cpp
    CCrypto/src/xor.cpp
	CCrypto/src/SslWrapper.cpp
)
# Header directory. PUBLIC: applies to the library and to whoever links it,
# so the executable can #include "Utils.h" without extra options
target_include_directories(CCrypto PUBLIC CCrypto/src)
# Warnings for this library only (PRIVATE: not propagated to its users)
target_compile_options(CCrypto PRIVATE -Wall -Wextra)

# --- Executable with all the challenges ---
# Collect every .cpp in Challenges/set1, set2, ...
# CONFIGURE_DEPENDS: adding a file is picked up by the next build
file(GLOB CHALLENGE_SOURCES CONFIGURE_DEPENDS Challenges/set*/*.cpp)

# A single executable: main.cpp plus all the challenges
add_executable(cryptopals Challenges/main.cpp ${CHALLENGE_SOURCES})
# Allows #include "set1/S01C01.h" in main.cpp
target_include_directories(cryptopals PRIVATE Challenges)
target_compile_options(cryptopals PRIVATE -Wall -Wextra)

# Linking: builds CCrypto first, then links it together with OpenSSL
target_link_libraries(cryptopals PRIVATE CCrypto OpenSSL::Crypto)
