# Makefile for Modular Simulation Framework
# Wraps Meson build system for convenience

BUILD_DIR = build

.PHONY: all configure build clean test install reconfigure help

# Default target
all: build

# Configure the build directory
configure:
	meson setup $(BUILD_DIR)

# Build the project
build: configure
	meson compile -C $(BUILD_DIR)

# Clean build artifacts
clean:
	rm -rf $(BUILD_DIR)

# Run tests
test: build
	meson test -C $(BUILD_DIR) --print-errorlogs

# Install the project
install: build
	meson install -C $(BUILD_DIR)

# Reconfigure the build
reconfigure:
	meson setup --reconfigure $(BUILD_DIR)

# Show help
help:
	@echo "Available targets:"
	@echo "  make             - Configure and build the project"
	@echo "  make build       - Build the project"
	@echo "  make clean       - Remove build directory"
	@echo "  make test        - Run tests"
	@echo "  make install     - Install the project"
	@echo "  make reconfigure - Reconfigure the build"
	@echo "  make help        - Show this help message"
