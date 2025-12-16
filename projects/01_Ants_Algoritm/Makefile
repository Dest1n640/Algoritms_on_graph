# --- Compiler and Flags ---
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2

# --- Directories ---
SRC_DIR := src
INCLUDE_DIR := include
BUILD_DIR := build

LIB_DIR := ../../libs/cpp_graph
LIB_INCLUDE_DIR := $(LIB_DIR)/include
LIB_SRC_DIR := $(LIB_DIR)/src

# --- Includes ---
INCLUDES := -I$(INCLUDE_DIR) -I$(LIB_INCLUDE_DIR)

# --- Source Files ---
SOURCES := $(wildcard $(SRC_DIR)/*.cpp)
LIB_SOURCES := $(wildcard $(LIB_SRC_DIR)/*.cpp)

# --- Object Files ---
OBJECTS := $(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(SOURCES))
LIB_OBJECTS := $(patsubst $(LIB_SRC_DIR)/%.cpp,$(BUILD_DIR)/lib_%.o,$(LIB_SOURCES))

# --- Target ---
TARGET := $(BUILD_DIR)/ant_colony.exe

# --- Detect OS ---
ifeq ($(OS),Windows_NT)
	MKDIR = if not exist "$(BUILD_DIR)" mkdir "$(BUILD_DIR)"
	RM = if exist "$(BUILD_DIR)" rmdir /S /Q "$(BUILD_DIR)"
else
	MKDIR = mkdir -p $(BUILD_DIR)
	RM = rm -rf $(BUILD_DIR)
endif

# --- Rules ---
.PHONY: all clean run

all: $(TARGET)

$(TARGET): $(OBJECTS) $(LIB_OBJECTS)
	@echo "Linking target: $@"
	@$(MKDIR)
	$(CXX) $(CXXFLAGS) -o $@ $^
	@echo "Build complete: $@"

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	@echo "Compiling: $<"
	@$(MKDIR)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

$(BUILD_DIR)/lib_%.o: $(LIB_SRC_DIR)/%.cpp
	@echo "Compiling library source: $<"
	@$(MKDIR)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

run: $(TARGET)
	@echo "Running $(TARGET)..."
	@$(TARGET) data/100.txt

clean:
	@echo "Cleaning up build files..."
	@$(RM)
