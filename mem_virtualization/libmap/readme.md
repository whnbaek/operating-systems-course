# libmap - Virtual Memory Mapping Library

## Overview
The `libmap` is part of the *Operating Systems Course* repository, focusing on virtual memory management. This library provides a foundational implementation for memory mapping using procfs, enabling users to explore concepts like virtual-to-physical address translation, memory protection, and efficient memory allocation using techniques such as `mmap`.

The library includes:
- Core functionalities for memory mapping.
- A demonstration program to showcase usage.
- A `Makefile` for building the project.

## Contents
The directory contains the following files:
- **`libmap.c`**: Implementation of the memory mapping functions.
- **`libmap.h`**: Header file defining the API for `libmap`.
- **`demo.c`**: A demonstration program illustrating how to use the library.
- **`Makefile`**: Build instructions for compiling the library and demo.

## Features
The library provides **Virtual Memory Mapping**, i.e., Functions to map virtual addresses to physical memory.


## Usage

### Prerequisites
Ensure you have a Linux-based system with GCC installed for compilation. The library relies on system calls like `mmap`, which are specific to Unix-like operating systems.

### Building the Project
Run the following command in the terminal:
```
make
```
This will compile both the library (`libmap`) and the demonstration program (`demo`).

### Running the Demo
After building, execute the demo program:
```
./demo
```
The demo showcases how to use the library's API for virtual memory mapping.

### Integration in Other Projects
To use `libmap` in your own projects:
1. Include `libmap.h` in your source files.
2. Link against `libmap.o` during compilation.

Example:
```
#include "libmap.h"

int main() {
    // Example usage of libmap functions
    return 0;
}
```

Compile with:
```
gcc -L${PATH_TO_LIBMAP_BUILD}/libmap -lmap -o my_program my_program.c 
```

## API Reference
Refer to `libmap.h` for detailed function prototypes and documentation.

Explore and experiment with virtual memory concepts using `libmap`. Happy coding!
