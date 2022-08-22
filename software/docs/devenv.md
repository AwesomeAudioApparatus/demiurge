
# Command-line
`PATH` must be point to the RISC-V embedded `gcc` which is called `riscv-none-embed-gcc`. 

If you have installed MounRiver, then just add its `bin/` directory to the `PATH`

Otherwise,

1. Download GNU GCC toolchain for Embedded RISC-V. We use; https://github.com/xpack-dev-tools/riscv-none-embed-gcc-xpack/releases/
1. Unpack to some directory and set the PATH for the `xpack-riscv-none-embed-gcc-10.2.0-1.2/bin/` (or equiv).

## Build
First time run;
```shell
cmake .
```

which will build the Make system, then simply run 
```shell
make
```
And the output files will be in `?`


# MounRiver IDE
We will try to keep the `.cproject`, `.project` and `.template` files current with development, but
contact us if something isn't right.

When building with MounRiver, you have 2 Build Profiles available;
1. release
2. debug

They will produce `.hex` and `.bin` files in `build-debug/` and `build-release` respectively.
