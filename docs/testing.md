# PageSim Testing and Validation

## 1. Testing Overview

PageSim uses multiple testing and debugging techniques:

- Automated functional testing
- GCC compiler warnings
- GDB debugging
- Valgrind memory analysis
- Manual simulation testing

The goal is to verify both correctness and memory safety.

---

## 2. Build Testing

PageSim is compiled using GCC with strict warning options:

```bash
make clean
make
