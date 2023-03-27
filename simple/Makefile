.PHONY: all clean openclsimple.dll main.exe

COMPILER := x86_64-w64-mingw32-g++-posix
FLAGS := -O2 -fno-rtti -std=c++17 -Wall -Wextra
MORE_FLAGS ?=

all: openclsimple.dll main.exe
	cp -afv $< ~/.mt5/drive_c/Program\ Files/MetaTrader\ 5/MQL5/Libraries/

openclsimple.dll: util.o openclsimple.o
	@# check if symbolic link file to wine's opencl.dll exists, if not then create one
	test -h opencl.dll && echo "opencl.dll exists, no need to create symbolic link again" || ln -s ~/.mt5/drive_c/windows/system32/opencl.dll ./opencl.dll
	$(COMPILER) -shared $(FLAGS) $(MORE_FLAGS) -fPIC -o $@ $^ -L. -lopencl

openclsimple.o: openclsimple.cpp openclsimple.h
	$(COMPILER) $(FLAGS) $(MORE_FLAGS) -DCLSIMPLE_API_EXPORT -DWINDOWS -I. -fPIC -o $@ -c $<

util.o: util.cpp util.h
	$(COMPILER) $(FLAGS) $(MORE_FLAGS) -I. -fPIC -o $@ -c $<

main.exe: main.cpp openclsimple.dll
	$(COMPILER) $(FLAGS) $(MORE_FLAGS) -I. -DWINDOWS -o $@ $< -L. -lopenclsimple

clean:
	rm -f openclsimple.dll main.exe opencl.dll util.o openclsimple.o
