EXE_NAME   := CCryptopalSolution
BIN_DIR    := bin
BUILD_DIR  := build
LIB_DIR    := lib/CCrypto
LIBNAME    := libCCrypto.a

# Directory dove cercare i file sorgente
SRC_DIRS   := Challenges

# Trova automaticamente tutti i .cpp nelle directory specificate
SOURCES    := $(foreach dir,$(SRC_DIRS),$(wildcard $(dir)/*.cpp))
OBJECTS    := $(SOURCES:%.cpp=$(BUILD_DIR)/%.o)

# Compiler options and flags
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra
DEBUGFLAGS = -g -DDEBUG
INCLUDE_DIR = -Iinclude -Ilib/CCrypto/include

# Default target
all: $(BIN_DIR)/$(EXE_NAME)

# ------------------------- Create the necessary directory -------------------------
$(BIN_DIR):
	@mkdir -p $(BIN_DIR)

$(BUILD_DIR)/%/:
	@mkdir -p $@


# Build of the library
$(LIB_DIR)/$(LIBNAME):
	@echo " [MAKE] Building the CCrypto lib"
	@$(MAKE) -C $(LIB_DIR)

# Link dell'eseguibile
$(BIN_DIR)/$(EXE_NAME): $(OBJECTS) $(LIB_DIR)/$(LIBNAME) | $(BIN_DIR)
	@echo " [MKDIR] Creation bin directory"
	@echo " [LINK] $@"
	@$(CXX) $(CXXFLAGS) -o $@ $(OBJECTS) -L$(LIB_DIR)/lib -lCCrypto

# Pattern rule per compilare tutti i .cpp nelle directory specificate
$(BUILD_DIR)/%.o: %.cpp
	@echo " [CXX] $< -> $@"
	@mkdir -p $(dir $@)
	@$(CXX) $(CXXFLAGS) $(INCLUDE_DIR) -c $< -o $@

# Cleaning target
clean:
	@$(MAKE) -C $(LIB_DIR) clean
	@echo " [CLEAN APP]"
	@rm -rf $(BUILD_DIR) $(BIN_DIR)

# Debug target
debug: CXXFLAGS += $(DEBUGFLAGS)
debug: $(BIN_DIR)/$(EXE_NAME)

# Target per vedere i valori delle variabili (debug del Makefile)
show-vars:
	@echo "SRC_DIRS: $(SRC_DIRS)"
	@echo "SOURCES: $(SOURCES)"
	@echo "OBJECTS: $(OBJECTS)"
	@echo "EXE_NAME: $(EXE_NAME)"



.PHONY: all clean debug show-vars
