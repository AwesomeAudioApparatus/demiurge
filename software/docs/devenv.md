
# Command-line
Download the GNU C Compiler for RISC-V from [their GitHub repository](https://github.com/xpack-dev-tools/riscv-none-elf-gcc-xpack/releases)
and unzip/install somewhere on your system.

Set up `PATH`to point to the `bin/` directory of the above. 

## Build
Simply run 
```shell
make
```
And the output files will be in CMakeFiles/ in the form of `demiurge.bin` and `demiurge.hex` files.

# MounRiver IDE
We will try to keep the `.cproject`, `.project` and `.template` files current with development, but
contact us if something isn't right.

When building with MounRiver, you have 2 Build Profiles available;
1. release
2. debug

They will produce `.hex` and `.bin` files in `build-debug/` and `build-release/` respectively.

NOTE: MounRiver will FAIL if the command-line build has been used.
