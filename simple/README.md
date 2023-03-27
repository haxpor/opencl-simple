# simple

## How to build and launch

### Cross-compilation

Execute the following command as per your need

* `make` - to build the program normally
* `make MORE_FLAGS=-DENABLE_DEBUG` - to build the program while enabled debugging information printing out
* `WINEPREFIX=~/.mt5 wine ./main.exe` - to launch the program

### Linux

Execute the following command as per your need

* `make -f Makefile-g++` - to build the program normally
* `make -f Makefile-g++ MORE_FLAGS=-DENABLE_DEBUG` - to build the program while enabled debugging information printing out
* `./main.out` - to launch the program
