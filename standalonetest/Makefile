.PHONY: all clean main.exe

COMPILER := x86_64-w64-mingw32-g++-posix
FLAGS := -O2 -fno-rtti -std=c++17 -Wall -Wextra
MORE_FLAGS ?=

all: main.exe

main.exe: opencltest.cpp
	$(COMPILER) $(FLAGS) $(MORE_FLAGS) -I. -o $@ $< -L. -lopencl

clean:
	rm -f main.exe
