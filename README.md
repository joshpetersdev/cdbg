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

---

## Build Instructions

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

### Install
```bash
sudo make install
```
Installs to:
- ```bash /usr/local/bin/cdbg```
- ```bash /usr/local/include/cdbg```

### Clean build artifacts
```bash
make clean
```
