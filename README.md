# opencl-simple
Testing ground of OpenCL 1.2 with its C++ API

It contains two categories of testing

1. standalonetest
2. simple

`standalonetest` is a simple cross-compiled executable to test using GPU with OpenCL in order to
execute a kernel function.

`simple` is a DLL + main program as a cross-compiled to test on Linux/Windows, and MT5 platform.

# Pre-requisite

Install the following debian-based package

* `mingw-w64`
* `winehq-devel` (or `winehq-stable` or `winehq-staging`, but recommend to use `-devel` package with MT5)
* MetaTrader 5

# Note

Those `.dll` files in each directory is a sym-link file to installed `.dll` via Mingw64, and Wine on
users' Linux system. See following detail.

* `libgcc_s_seh-1.dll` - as part of Mingw64
* `libstdc++-6.dll` - as part of Mingw64
* `libwinpthread-1.dll` - as part of Mingw64

Those symlink files points to the wine prefix as of `~/.mt5` which is the default
installation path of MetaTrader 5. In case you install into different path, you
have to update its location via the following command line.

```
ln -sf <new-path> ./libgcc_s_seh-1.dll
```

to update path that symlink file `libgcc_s_seh-1.dll` would be pointing to to `<new-path>`.

# How to build

See `README.md` of each sub-project.

# License
GNU GPL-3.0, Wasin Thonkaew
