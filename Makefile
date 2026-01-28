CXX := clang++
CXXFLAGS := -std=c++14 -Wall -Wextra -Wpedantic
INCLUDES := -Iinclude

# Build configurations
DEBUG_FLAGS := -O0 -g -DDEBUG
RELEASE_FLAGS := -O3 -DNDEBUG -march=native
BENCHMARK_FLAGS := -O3 -DNDEBUG -march=native -fno-omit-frame-pointer

# Default to release for performance work
BUILD_FLAGS := $(RELEASE_FLAGS)

# Directories
SRC_DIR := src
BUILD_DIR := build
BIN_DIR := $(BUILD_DIR)/bin
RESULTS_DIR := results

# Find all source files
SRCS := $(wildcard $(SRC_DIR)/*/*.cpp)
BINS := $(patsubst $(SRC_DIR)/%.cpp,$(BIN_DIR)/%,$(SRCS))

.PHONY: all clean setup debug release benchmark help list run

all: setup $(BINS)

setup:
	@mkdir -p $(BIN_DIR) $(RESULTS_DIR)
	@mkdir -p $(BIN_DIR)/month1 $(BIN_DIR)/month2 $(BIN_DIR)/month3

# Build each source file
$(BIN_DIR)/%: $(SRC_DIR)/%.cpp | setup
	@mkdir -p $(dir $@)
	@echo "Building: $(notdir $<)"
	$(CXX) $(CXXFLAGS) $(BUILD_FLAGS) $(INCLUDES) $< -o $@

# Build modes
debug:
	$(MAKE) BUILD_FLAGS="$(DEBUG_FLAGS)"

release:
	$(MAKE) BUILD_FLAGS="$(RELEASE_FLAGS)"

# Run specific day (e.g., make run DAY=month1/week01/day01)
run:
	@if [ -z "$(DAY)" ]; then echo "Usage: make run DAY=month1/week01/day01"; exit 1; fi
	@echo "=== Building $(DAY) ==="
	@$(MAKE) $(BIN_DIR)/$(DAY)
	@if [ -f "$(BIN_DIR)/$(DAY)" ]; then \
		echo "=== Running $(DAY) ==="; \
		./$(BIN_DIR)/$(DAY); \
	else \
		echo "Build failed for $(DAY)"; \
		exit 1; \
	fi

# List available days
list:
	@echo "Available learning modules:"
	@find $(SRC_DIR) -name "*.cpp" 2>/dev/null | sed 's|$(SRC_DIR)/||' | sed 's|\.cpp$$||' | sort || echo "No modules found yet"

clean:
	rm -rf $(BUILD_DIR) $(RESULTS_DIR)

help:
	@echo "C++ Mastery Build System"
	@echo ""
	@echo "Targets:"
	@echo "  all              Build all modules"
	@echo "  run DAY=path     Run specific day (e.g., month1/week01/day01)"
	@echo "  list             List available modules"
	@echo "  debug            Build with debug flags"
	@echo "  release          Build with release flags (default)"
	@echo "  benchmark        Build with benchmark optimization"
	@echo "  clean            Clean build artifacts"
	@echo "  help             Show this help"