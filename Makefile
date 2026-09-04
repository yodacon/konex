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

## deps    - install the toolchain and libraries (Homebrew on macOS, apk on Alpine)
deps:
	@if command -v brew >/dev/null 2>&1; then \
	  brew install cmake sdl2; \
	  echo "Xcode Command Line Tools: xcode-select --install (if missing)"; \
	elif command -v apk >/dev/null 2>&1; then \
	  echo "Alpine: the Linux build is X11 + GLX, no SDL2 and no OpenAL."; \
	  echo "  doas apk add build-base cmake libx11-dev mesa-dev glu-dev"; \
	else \
	  echo "No brew and no apk. The Linux build needs a C++ compiler, cmake,"; \
	  echo "and the X11, OpenGL and GLU development headers."; exit 1; \
	fi

## check   - report what the build needs and whether it is present
check:
	@printf '%-10s ' "c++:";     c++ --version 2>/dev/null | head -1 || echo MISSING
	@printf '%-10s ' "cmake:";   cmake --version 2>/dev/null | head -1 || echo MISSING
	@if [ "$$(uname -s)" = Darwin ]; then \
	  printf '%-10s ' "sdl2:";   sdl2-config --version 2>/dev/null || echo MISSING; \
	else \
	  printf '%-10s ' "x11:";    pkg-config --modversion x11 2>/dev/null || echo MISSING; \
	  printf '%-10s ' "gl:";     pkg-config --modversion gl 2>/dev/null || echo MISSING; \
	  printf '%-10s ' "glu:";    pkg-config --modversion glu 2>/dev/null || echo MISSING; \
	fi
	@printf '%-10s ' "binary:";  test -x $(BIN) && echo "$(BIN)" || echo "not built"

## help    - list targets
help:
	@grep -hE '^## ' $(MAKEFILE_LIST) | sed 's/^## /  make /'
