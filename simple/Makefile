.PHONY: all clean openclsimple.dll main.exe

COMPILER := x86_64-w64-mingw32-g++-posix
FLAGS := -O2 -fno-rtti -std=c++17 -Wall -Wextra
MORE_FLAGS ?=

all: openclsimple.dll main.exe
	cp -afv $< /home/haxpor/.mt5/drive_c/Program\ Files/MetaTrader\ 5/MQL5/Libraries/

openclsimple.dll: openclsimple.cpp openclsimple.h
	$(COMPILER) -shared $(FLAGS) $(MORE_FLAGS) -DCLSIMPLE_API_EXPORT -DWINDOWS -I. -fPIC -o $@ $< -L. -lopencl

main.exe: main.cpp openclsimple.dll
	$(COMPILER) $(FLAGS) $(MORE_FLAGS) -I. -DWINDOWS -o $@ $< -L. -lopenclsimple

clean:
	rm -f openclsimple.dll main.exe
