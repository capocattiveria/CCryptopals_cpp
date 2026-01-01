# Build Configuration(default: release)
# Use: make BUILD=debug to build in debug mode
BUILD ?= release

# Library name
LIBNAME := libCCrypto.a

# Directory
SRC_DIR     := src
INCLUDE_DIR := include
LIB_DIR     := lib

# Compiler
CXX := g++

# Different directory for debug and release
ifeq ($(BUILD),debug)
    BUILD_DIR := build/debug
    LIB_OUT   := $(LIB_DIR)/debug
    CXXFLAGS  := -g -O0 -DDEBUG -Wall -Wextra -std=c++23
else
    BUILD_DIR := build/release
    LIB_OUT   := $(LIB_DIR)/release
    CXXFLAGS  := -O2 -DNDEBUG -Wall -Wextra -std=c++23
endif

# File sorgenti
SOURCES := StringConverter.cpp Utils.cpp xor.cpp
SRC_FILES := $(addprefix $(SRC_DIR)/, $(SOURCES))
OBJECTS := $(addprefix $(BUILD_DIR)/, $(SOURCES:.cpp=.o))

# Library target
LIB_TARGET := $(LIB_OUT)/$(LIBNAME)

# Default target
.DEFAULT_GOAL := all

# Phony targets
.PHONY: all debug release clean clean-all help

all: $(LIB_TARGET)
	@echo " [ OK ] $(BUILD) build completata: $(LIB_OUT)/$(LIBNAME)"

# Explicit build
debug:
	@$(MAKE) BUILD=debug all

release:
	@$(MAKE) BUILD=release all

# Create both the build.
both: clean-all
	@echo "=== Building DEBUG ==="
	@$(MAKE) BUILD=debug
	@echo ""
	@echo "=== Building RELEASE ==="
	@$(MAKE) BUILD=release
	@echo ""
	@echo " [ OK ] Entrambe le build completate"
	@echo "        Debug:   $(LIB_DIR)/debug/$(LIBNAME)"
	@echo "        Release: $(LIB_DIR)/release/$(LIBNAME)"

# Creating static library
$(LIB_TARGET): $(OBJECTS) | $(LIB_OUT)
	@echo " [ AR  ] $(BUILD): $@"
	@ar rcs $@ $^

# Creating output directory
$(LIB_OUT):
	@echo " [MKDIR] $@"
	@mkdir -p $(LIB_OUT)

# Compiling object
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp | $(BUILD_DIR)
	@echo " [ CXX ] $(BUILD): $< -> $@"
	$(CXX) $(CXXFLAGS) -c $< -o $@ -I$(INCLUDE_DIR)

# Creazione directory build
$(BUILD_DIR):
	@echo " [MKDIR] $@"
	@mkdir -p $(BUILD_DIR)

# Pulizia build corrente
clean:
	@echo " [CLEAN] $(BUILD_DIR) e $(LIB_OUT)"
	@rm -rf $(BUILD_DIR) $(LIB_OUT)

# completa (debug + release)
clean-all:
	@echo " [CLEAN] Rimozione di tutte le build"
	@rm -rf build lib

# Show information
info:
	@echo "Current configuration:"
	@echo "  BUILD:     $(BUILD)"
	@echo "  BUILD_DIR: $(BUILD_DIR)"
	@echo "  LIB_OUT:   $(LIB_OUT)"
	@echo "  CXXFLAGS:  $(CXXFLAGS)"
	@echo "  OBJECTS:   $(OBJECTS)"

help:
	@echo "Makefile for libCCrypto.a"
	@echo ""
	@echo "Available target:"
	@echo "  make              - Build release (default)"
	@echo "  make debug        - Build debug with symbols (-g -O0)"
	@echo "  make release      - Build release optimized(-O2)"
	@echo "  make both         - Build with both debug and release"
	@echo "  make clean        - Clean the current build"
	@echo "  make clean-all    - Clean all builds"
	@echo "  make info         - Show the current configuration"
	@echo ""
	@echo "Examble:"
	@echo "  make BUILD=debug  - Build in debug mode"
	@echo "  make BUILD=release- Build in release mode"
