# IJVM Emulator

This project is an implementation of an **Integer Java Virtual Machine (IJVM)** emulator, capable of interpreting and executing binary code compiled for the IJVM architecture.
For the original documentation, see: [http://vu-programming.gitlab.io/pad/website/](http://vu-programming.gitlab.io/pad/website/)

---

## Implementation Overview

* **Stack-based Execution:** Handling of the **Operand Stack (OS)** for computation and the **Local Variable Frame (LV)** for local data storage. 
* **Constant Pool (CP):** Loading and utilizing the constant pool for accessing string constants, method references, and other static data.
* **Method Invocation:** Handling `INVOKEVIRTUAL` (method calling) and `IRETURN` (integer return).
* **All Mandated Instructions:** Emulation of the complete set of required IJVM instructions (e.g., `BIPUSH`, `IADD`, `DUP`, `GOTO`, `IFLT`, etc.).

---

##  Execution


### Prerequisites

**make** and a **C11-compliant compiler** (such as GCC or Clang)

### Building the Emulator

Navigate to the project directory and run the following command to compile the source code:
```bash
make ijvm
```

### Running an IJVM Binary
To execute an IJVM program, run the compiled emulator, passing the path to the .ijvm binary file as a command-line argument:
```bash
./ijvm <path/to/your/binary.ijvm>
```

### Debugging and Testing

#### Debug Output

You can enable verbose debug printing by recompiling with the -DDEBUG flag. This prints the state information (e.g., register values, stack contents) during execution (the debug function is defined in include/util.h).
```bash
make clean && make ijvm CFLAGS=-DDEBUG
```

#### Automated Testing
The following make commands are available for verification and quality assurance:

Run all basic sanity tests.
```bash
make testbasic
```

Run all advanced instruction and program tests.
```bash
make testadvanced
```

Check for memory leaks using Valgrind.
```bash
make testleaks
```

Check for memory errors/undefined behavior (requires LLVM).
```bash
make testsanitizers
```

