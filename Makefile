BUILD ?= debug

SET ?= 1
CHALLENGE ?= 5

CHALLENGE_DIR := Challenges
BIN_DIR := bin
INCLUDE_DIR := CCrypto/src
CXX := g++

ifeq ($(BUILD),debug)
    LIB := CCrypto/lib/debug/libCCrypto.a
    BIN_OUT := $(BIN_DIR)/debug
    CXXFLAGS := -g -O0 -Wall -std=c++23
    BUILD_DEF := -DDEBUG
else
    LIB := CCrypto/lib/release/libCCrypto.a
    BIN_OUT := $(BIN_DIR)/release
    CXXFLAGS := -O2 -Wall -std=c++23
    BUILD_DEF := -DNDEBUG
endif

MAIN_SRC := $(CHALLENGE_DIR)/main.cpp

#Naming the executable following the set and challenge compiled
TARGET := $(BIN_OUT)/cryptopalsS$(SET)C$(CHALLENGE) 

# Get the sources under the Set directory
CHALLENGE_SOURCES := $(wildcard $(CHALLENGE_DIR)/set$(SET)/*.cpp)

.PHONY: all debug release clean list compile_commands run refresh_lsp clean-all $(LIB)

all: $(LIB) $(TARGET)

debug:
	@$(MAKE) BUILD=debug

release:
	@$(MAKE) BUILD=release

$(LIB):
	@echo "Make lib"
	@$(MAKE) -C CCrypto BUILD=$(BUILD)

$(BIN_OUT):
	@mkdir -p $@

$(TARGET): $(MAIN_SRC) $(CHALLENGE_SOURCES) $(LIB) refresh_lsp | $(BIN_OUT)
	@echo "Building $@ (SET=$(SET) CHALLENGE=$(CHALLENGE))"
	@$(CXX) $(CXXFLAGS) $(BUILD_DEF) -DSET=$(SET) -DCHALLENGE=$(CHALLENGE) \
		$(MAIN_SRC) $(CHALLENGE_SOURCES) -o $@ \
		-I$(INCLUDE_DIR) -I$(CHALLENGE_DIR) $(LIB)

clean:
	@$(MAKE) -C CCrypto clean
	@rm -rf $(BIN_DIR)

clean-all:
	@$(MAKE) -C CCrypto clean-all
	@rm -rf build bin 

list:
	@echo "Available sets:"
	@find $(CHALLENGE_DIR) -mindepth 1 -maxdepth 1 -type d -exec basename {} \; | sort

help:
	@echo "Cryptopals Challenge Build System"
	@echo ""
	@echo "Usage:"
	@echo "  make [target] [SET=n] [CHALLENGE=n] [BUILD=debug|release]"
	@echo ""
	@echo "Targets:"
	@echo "  all              Build challenge (default: SET=1 CHALLENGE=1 BUILD=debug)"
	@echo "  debug            Build in debug mode"
	@echo "  release          Build in release mode"
	@echo "  compile_commands Generate compile_commands.json with bear"
	@echo "  clean            Remove all build artifacts"
	@echo "  list             List available challenge sets"
	@echo "  help             Show this help message"
	@echo "  clean-all	  Clean all the builds"
	@echo ""
	@echo "Variables:"
	@echo "  SET=n            Challenge set number (default: 1)"
	@echo "  CHALLENGE=n      Challenge number (default: 1)"
	@echo "  BUILD=type       Build type: debug or release (default: debug)"
	@echo ""
	@echo "Examples:"
	@echo "  make                           # Build set1, challenge1 (debug)"
	@echo "  make SET=2 CHALLENGE=3         # Build set2, challenge3 (debug)"
	@echo "  make run SET=1 CHALLENGE=5     # Build and run set1, challenge5"
	@echo "  make release SET=3 CHALLENGE=1 # Build set3, challenge1 (release)"
	@echo "  make clean                     # Clean all builds"
	@echo ""
	@echo "Output:"
	@echo "  Debug:   $(BIN_DIR)/debug/cryptopalsS<SET>C<CHALLENGE>"
	@echo "  Release: $(BIN_DIR)/release/cryptopalsS<SET>C<CHALLENGE>"


compile_commands:
	@bear -- $(MAKE) clean all

refresh_lsp:
	@rm compile_commands.json
	@echo "Refreshing LSP compile commands for SET=$(SET) CHALLENGE=$(CHALLENGE)..."
	@bear --append -- $(CXX) $(CXXFLAGS) $(BUILD_DEF) -DSET=$(SET) -DCHALLENGE=$(CHALLENGE) \
		$(MAIN_SRC) $(CHALLENGE_SOURCES) -fsyntax-only \
		-I$(INCLUDE_DIR) -I$(CHALLENGE_DIR)


