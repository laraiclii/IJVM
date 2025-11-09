# IJVM Emulator

This project is a complete implementation of an **Integer Java Virtual Machine (IJVM)** emulator, capable of interpreting and executing binary code compiled for the IJVM architecture.
For the original documentation, see: [http://vu-programming.gitlab.io/pad/website/](http://vu-programming.gitlab.io/pad/website/)

---

## Implementation Overview

The emulator fully implements the **IJVM specification** (a subset of the JVM), focusing on stack-based execution and constant pool manipulation.

Key implemented features include:

* **Stack-based Execution:** Correct handling of the **Operand Stack (OS)** for computation and the **Local Variable Frame (LV)** for local data storage. 
* **Constant Pool (CP):** Loading and utilizing the constant pool for accessing string constants, method references, and other static data.
* **Method Invocation:** Full support for `INVOKEVIRTUAL` (method calling) and `IRETURN` (integer return).
* **All Mandated Instructions:** Faithful emulation of the complete set of required IJVM instructions (e.g., `BIPUSH`, `IADD`, `DUP`, `GOTO`, `IFLT`, etc.).

---

##  Execution

Follow these steps to build and run the finished IJVM emulator with a binary program.

### Prerequisites

You need **make** and a **C11-compliant compiler** (such as GCC or Clang) installed on your system.

### 1. Building the Emulator

Navigate to the project directory and run the following command to compile the source code:
```bash
make ijvm
```

### 2. Running an IJVM Binary
To execute an IJVM program, run the compiled emulator, passing the path to the .ijvm binary file as a command-line argument:
```bash
./ijvm <path/to/your/binary.ijvm>
```

### 2. Debugging and Testing

#### Debug Output

You can enable verbose debug printing by recompiling with the -DDEBUG flag. This prints the state information (e.g., register values, stack contents) during execution (the debug function is defined in include/util.h).
```bash
make clean && make ijvm CFLAGS=-DDEBUG
```

#### Automated Testing
The following make commands are available for verification and quality assurance:

Command,Description
make testbasic,Run all basic sanity tests.
make testadvanced,Run all advanced instruction and program tests.
make testleaks,Check for memory leaks using Valgrind.
make testsanitizers,Check for memory errors/undefined behavior (requires LLVM). 
