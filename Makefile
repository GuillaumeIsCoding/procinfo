# Makefile to manage CMake compilation

# Variables
BUILD_DIR = builds
CMAKE = cmake
MAKE = make
TARGET = ProcInfo
LIBRARY = ProcInfoLib

# CMake options
CMAKE_BUILD_TYPE ?= Release
BUILD_SANDBOX ?= OFF

# Colors for display
GREEN = \033[0;32m
YELLOW = \033[0;33m
RED = \033[0;31m
BLUE = \033[0;34m
NC = \033[0m # No Color

.PHONY: all clean rebuild run help sandbox library configure

# Default compilation (library only)
all: $(BUILD_DIR)
	@echo "$(GREEN)Compiling library...$(NC)"
	@cd $(BUILD_DIR) && $(MAKE)
	@echo "$(GREEN)Compilation successful!$(NC)"

# Create builds directory and configure CMake
$(BUILD_DIR):
	@echo "$(YELLOW)Configuring CMake...$(NC)"
	@mkdir -p $(BUILD_DIR)
	@cd $(BUILD_DIR) && $(CMAKE) -DCMAKE_BUILD_TYPE=$(CMAKE_BUILD_TYPE) \
		-DBUILD_SANDBOX=$(BUILD_SANDBOX) ..
	@echo "$(GREEN)Configuration complete!$(NC)"

# Configure with specific options
configure:
	@echo "$(YELLOW)Reconfiguring CMake...$(NC)"
	@mkdir -p $(BUILD_DIR)
	@cd $(BUILD_DIR) && $(CMAKE) -DCMAKE_BUILD_TYPE=$(CMAKE_BUILD_TYPE) \
		-DBUILD_SANDBOX=$(BUILD_SANDBOX) ..
	@echo "$(GREEN)Configuration complete!$(NC)"

# Build library only
library: $(BUILD_DIR)
	@echo "$(GREEN)Compiling library...$(NC)"
	@cd $(BUILD_DIR) && $(MAKE) $(LIBRARY)
	@echo "$(GREEN)Library compilation successful!$(NC)"

# Build with sandbox enabled
sandbox:
	@echo "$(BLUE)Building with sandbox enabled...$(NC)"
	@$(MAKE) configure BUILD_SANDBOX=ON
	@cd $(BUILD_DIR) && $(MAKE)
	@echo "$(GREEN)Sandbox build successful!$(NC)"

# Clean compilation files
clean:
	@echo "$(RED)Cleaning compilation files...$(NC)"
	@rm -rf $(BUILD_DIR)
	@echo "$(GREEN)Cleaning complete!$(NC)"

# Rebuild from scratch
rebuild: clean all

# Run the sandbox program
run: sandbox
	@echo "$(GREEN)Running $(TARGET)...$(NC)"
	@./$(BUILD_DIR)/$(TARGET)

# Display help
help:
	@echo "$(YELLOW)Available commands:$(NC)"
	@echo "  make                    - Compile the library"
	@echo "  make library            - Compile library only"
	@echo "  make sandbox            - Compile with sandbox executable"
	@echo "  make clean              - Clean compilation files"
	@echo "  make rebuild            - Rebuild from scratch"
	@echo "  make run                - Compile sandbox and run the program"
	@echo "  make configure          - Reconfigure CMake"
	@echo "  make help               - Display this help"
	@echo ""
	@echo "$(YELLOW)Options:$(NC)"
	@echo "  BUILD_SANDBOX=ON/OFF    - Enable/disable sandbox build (default: OFF)"
	@echo "  CMAKE_BUILD_TYPE=TYPE   - Set build type: Debug/Release (default: Release)"
	@echo ""
	@echo "$(BLUE)Examples:$(NC)"
	@echo "  make BUILD_SANDBOX=ON             - Build with sandbox"
	@echo "  make CMAKE_BUILD_TYPE=Debug       - Build in debug mode"
	@echo "  make sandbox CMAKE_BUILD_TYPE=Debug - Build sandbox in debug mode"