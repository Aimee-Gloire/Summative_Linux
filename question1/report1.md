# Question 1 — ELF Binary Reverse Engineering Analysis

## 1. Overview

This task required performing a reverse-engineering analysis of a provided ELF binary.  
Using **objdump**, **strace**, and **gdb**, the goal was to identify:

- Functions and their control flow  
- Conditional/unconditional jumps  
- System calls used  
- Memory usage patterns  
- Runtime behaviour  

All outputs (objdump, strace, gdb logs) are included in this folder.

---

## 2. Tools Used

- `objdump -d -M intel question1 > objdump_q1.txt`
- `strace -f ./question1 2>&1 | tee strace_q1.txt`
- `gdb -q question1` with logging

---

## 3. Functions Identified (from objdump)

From `objdump_q1.txt`, the binary contains:

- `main`
- `getStudentNames`
- `writeToFile`
- `readFromFile`
- `sortNames`
- `printNames`

In addition, common PLT stubs appear:

- `printf@plt`
- `fgets@plt`
- `fopen@plt`
- `fclose@plt`
- `strcmp@plt`
- `fprintf@plt`

---

## 4. Control Flow Summary

The program follows this sequence:

1. **main**  
   Allocates a large stack buffer and calls `getStudentNames`.

2. **getStudentNames**  
   Reads names from input using `fgets`. Returns to main.

3. **writeToFile**  
   Opens an output file and writes collected names.

4. **readFromFile**  
   Reads names back from the file into memory.

5. **sortNames**  
   Implements a **nested loop** sorting routine.
   - Includes conditional jumps such as `jl` (jump if less) used to compare strings.
   - Unconditional `jmp` instructions form the loop structure.

6. **printNames**  
   Prints sorted names.

---

## 5. Memory Access Summary

Analysis via objdump + gdb indicates:

- A large local buffer allocated via  
  `sub rsp, 0x3f0`  
  → **Stack allocation**, no heap allocation.
- A **stack canary** is present:  
  The code reads and verifies from `fs:0x28` at function exit.  
  This protects against stack smashing.
- File I/O uses libc (fopen, fwrite, fclose).
- Strings are stored and manipulated **entirely on the stack**.

---

## 6. System Calls Summary (from strace)

`strace_q1.txt` reveals the following calls:

- `execve` — load program
- `openat` — open input/output files
- `read` / `write` — read file contents and print output
- `fstat` — retrieve file information
- `close` — close file descriptor
- Memory-related: `mmap`, `brk` — standard libc startup routines

This confirms:

- The program interacts with **files** (open/read/write/close)
- Uses **console output** via write()
- Uses the standard ELF loader and libc initialization paths

---

## 7. GDB Execution Flow Summary

Breakpoints were set at:

- `main`
- `writeToFile`
- `readFromFile`
- `sortNames`

Observations:

- Registers show arguments passed via System V ABI (rdi, rsi, rdx).
- Memory inspection reveals:
  - Name arrays stored on stack
  - Both loops in `sortNames` operate on stack-stored pointers
- The program transitions cleanly between functions with no recursion.

---

## 8. How to Reproduce (WSL/Linux)

Run:

```bash
objdump -d -M intel ./question1 > objdump_q1.txt
strace -f ./question1 2>&1 | tee strace_q1.txt
gdb -q ./question1 :

set logging file gdb_q1_log.txt
set logging on
break main
run
continue
set logging off
quit
```
