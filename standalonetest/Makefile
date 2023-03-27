.PHONY: all clean main.exe

COMPILER := x86_64-w64-mingw32-g++-posix
FLAGS := -O2 -fno-rtti -std=c++17 -Wall -Wextra
MORE_FLAGS ?=

all: main.exe

main.exe: opencltest.cpp
	@# check if symbolic link file to wine's opencl.dll exists, if not then create one
	test -h opencl.dll && echo "opencl.dll exists, no need to create symbolic link again" || ln -s ~/.mt5/drive_c/windows/system32/opencl.dll ./opencl.dll
	$(COMPILER) $(FLAGS) $(MORE_FLAGS) -I. -o $@ $< -L. -lopencl

clean:
	rm -f main.exe
