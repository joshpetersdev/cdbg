# cdbg - A Linux Debugger in C 

cdbg is a minimal Linux debugger written in C using ptrace and ELF parsing concepts.
This project is build for learning systems programming, operating systems internals, and debugging techniques used in tools like gdb and lldb.

---

## Features (WIP)

- Basic process execution control (ptrace)
- Breakpoint support
- ELF binary inspection
- Simple test framework (no external dependencies)
- GCC + Make-based build system 
- TEST 2

---

## Build Instructions

### Install
```bash
sudo make install
```
Installs to:
- ```/usr/local/bin/cdbg```
- ```/usr/local/include/cdbg```

### Build the debugger
```bash
make 
```

### Run the debugger
```bash
./cdbg
```

### Run Tests
```bash
make tests 
```

### Clean build artifacts
```bash
make clean
```
