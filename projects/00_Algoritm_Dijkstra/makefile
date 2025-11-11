--
# Generate object file names in the build directory
OBJECTS := $(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(SOURCES))
LIB_OBJECTS := $(patsubst $(LIB_SRC_DIR)/%.cpp,$(BUILD_DIR)/lib_%.o,$(LIB_SOURCES))

# --- Target ---
TARGET := $(BUILD_DIR)/dijkstra.exe

# --- Detect OS ---
ifeq ($(OS),Windows_NT)
    MKDIR = if not exist "$(BUILD_DIR)" mkdir "$(BUILD_DIR)"
    RM = if exist "$(BUILD_DIR)" rmdir /S /Q "$(BUILD_DIR)"
else
    MKDIR = mkdir -p $(BUILD_DIR)
    RM = rm -rf $(BUILD_DIR)
endif

# --- Rules ---

# Phony targets do not represent actual files
.PHONY: all clean run

# Default target
all: $(TARGET)

# Rule to link the final executable
$(TARGET): $(OBJECTS) $(LIB_OBJECTS)
	@echo "Linking target: $@"
	@$(MKDIR)
	$(CXX) $(CXXFLAGS) -o $@ $^
	@echo "Build complete: $@"

# Rule to compile project source files into object files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	@echo "Compiling: $<"
	@$(MKDIR)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

# Rule to compile library source files into object files
$(BUILD_DIR)/lib_%.o: $(LIB_SRC_DIR)/%.cpp
	@echo "Compiling library source: $<"
	@$(MKDIR)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

# Rule to run the program
run: $(TARGET)
	@echo "Running $(TARGET)..."
	@$(TARGET)

# Rule to clean up build artifacts
clean:
	@echo "Cleaning up build files..."
	@$(RM)
