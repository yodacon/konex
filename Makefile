# Konex — convenience entry points for the CMake build.
#
# The original 2005 build was Autotools (Makefile.cvs -> aclocal/automake/
# autoconf -> configure -> Makefile). That chain no longer resolves against a
# modern macro set, so CMakeLists.txt is the real build definition and this
# Makefile is a thin front end over it.
#
# Note: `./configure` from the old Autotools path would generate a Makefile
# here and clobber this file. Don't mix the two build systems in one tree.

BUILD      ?= build
BUILD_TYPE ?= Debug
JOBS       ?= $(shell sysctl -n hw.ncpu 2>/dev/null || nproc 2>/dev/null || echo 4)
BIN         = $(BUILD)/bin/konex

.PHONY: all build run clean distclean rebuild release deps check help

all: build

## build   - configure if needed, then compile
build: $(BUILD)/CMakeCache.txt
	@cmake --build $(BUILD) -j$(JOBS)

$(BUILD)/CMakeCache.txt:
	@cmake -S . -B $(BUILD) -DCMAKE_BUILD_TYPE=$(BUILD_TYPE)

## run     - build, then launch from the staged asset directory
run: build
	@cd $(BUILD)/bin && ./konex

## release - optimized build in build-release/
release:
	@$(MAKE) build BUILD=build-release BUILD_TYPE=Release

## rebuild - wipe the build tree and build from scratch
rebuild: clean build

## clean   - remove the build tree
clean:
	@rm -rf $(BUILD)

## distclean - remove every build tree
distclean:
	@rm -rf build build-release

## deps    - install the macOS toolchain and libraries
deps:
	@command -v brew >/dev/null 2>&1 || { \
	  echo "Homebrew is required: https://brew.sh"; exit 1; }
	brew install cmake sdl2
	@echo "Xcode Command Line Tools: xcode-select --install (if missing)"

## check   - report what the build needs and whether it is present
check:
	@printf '%-10s ' "clang++:"; c++ --version 2>/dev/null | head -1 || echo MISSING
	@printf '%-10s ' "cmake:";   cmake --version 2>/dev/null | head -1 || echo MISSING
	@printf '%-10s ' "sdl2:";    sdl2-config --version 2>/dev/null || echo MISSING
	@printf '%-10s ' "binary:";  test -x $(BIN) && echo "$(BIN)" || echo "not built"

## help    - list targets
help:
	@grep -hE '^## ' $(MAKEFILE_LIST) | sed 's/^## /  make /'
