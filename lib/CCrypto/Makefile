# Building the library

LIBNAME := libCCrypto.a

# Directory dei sorgenti
SRC_DIR       := src
BUILD_DIR     := build
LIB_DIR       := lib
INCLUDE_DIR   := include
 
# Basename of the source files (solo i nomi)
SOURCES := StringConverter.cpp

# Percorsi completi dei sorgenti
SRC_FILES := $(addprefix $(SRC_DIR)/, $(SOURCES))

# The objects have the same name of the .cpp files
# Create a new lists variable starting from SOURCES
OBJECTS := $(addprefix $(BUILD_DIR)/, $(SOURCES:.cpp=.o))

# Debug: stampa le variabili
# $(info SOURCES: $(SOURCES))
# $(info OBJECTS: $(OBJECTS))
# $(info SRC_FILES: $(SRC_FILES))

# Compiler C++ and flags
CXX := g++
CXXFLAGS := -Wall -Wextra -std=c++17 -O2

all: $(LIB_DIR)/$(LIBNAME)
	@echo " [ all ok my friends ] You can find the $(LIBNAME) in the lib directory."

# Create the static library
$(LIB_DIR)/$(LIBNAME): $(OBJECTS) | $(LIB_DIR)
	@echo " [AR]  Library creation $@"
	@ar rcs $@ $^

$(LIB_DIR):
	@echo " [MKDIR] $@" 
	@mkdir -p $(LIB_DIR)

# Generic rule to compile .o from .cpp
# $@ = target (es: build/foo.o)
# $< = primo prerequisito (es: src/foo.cpp)
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp | $(BUILD_DIR)
	@echo " [CXX] Compilazione $< -> $@"
	@$(CXX) $(CXXFLAGS) -c $< -o $@ -I$(INCLUDE_DIR)

# Crea la cartella build se non esiste
$(BUILD_DIR):
	@echo " [MKDIR] $@" 
	@mkdir -p $(BUILD_DIR)


# Pulizia
clean:
	@echo " [CLEAN]"
	@rm -rf $(BUILD_DIR) $(LIB_DIR)


.PHONY: all clean
