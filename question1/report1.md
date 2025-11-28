# Question 1 — ELF Binary Analysis

## Tools used

- objdump -d -M intel
- strace -f ./question1 2>&1 | tee strace_q1.txt
- gdb -q question1 (with logging)

## Functions (from objdump)

- main
- getStudentNames
- writeToFile
- readFromFile
- sortNames
- printNames
- plus libc PLT entries (printf, fgets, fopen, fclose, strcmp, fprintf)

## Control flow

- main -> getStudentNames -> writeToFile -> readFromFile -> sortNames -> printNames -> exit
- sortNames uses nested loops; conditional jumps (`jl`) and unconditional `jmp` build the loops.

## Memory accesses

- Large local buffer on stack (allocated via `sub rsp,0x3f0`).
- Stack canary present (`fs:0x28` read/written).
- No `malloc` observed in static disassembly; file I/O via libc.

## Strace summary (observed)

- execve, openat, read, write, fstat, close, brk/mmap for libc.
- Confirms file I/O and console output via write(1,...).

## GDB summary (observed)

- Breakpoints hit at main, writeToFile, readFromFile, sortNames.
- Memory around $rbp shows strings written to stack buffers.
- Arguments passed in rdi/rsi/rdx consistent with x86-64 calling convention.

## How to reproduce locally (WSL)

1. Make binary executable:
   chmod +x question1/question1
2. Disassemble:
   objdump -d -M intel question1/question1 > question1/objdump_q1.txt
3. Capture strace:
   strace -f ./question1/question1 2>&1 | tee question1/strace_q1.txt
4. GDB log:
   gdb -q question1/question1
   (inside) set logging file question1/gdb_q1_log.txt
   set logging on
   break main
   run
   continue
   set logging off
   quit
