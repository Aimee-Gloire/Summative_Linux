# Question 2 — Assembly Sensor Line Counter

## 1. Overview

This task required writing an x86-64 assembly program (`sensor_count.asm`) that:

- Opens a text file named `sensor_log.txt`
- Reads its contents into memory
- Counts the total number of lines, including **empty lines**
- Prints the total count in the format:  
  `Total sensor readings: X`

The solution uses Linux syscalls and handles empty lines and trailing newline correctly.

---

## 2. Files

- sensor_count.asm  (NASM source)
- sensor_count      (executable produced locally)
- sensor_log.txt    (input)

---

## 3. Tools and Concepts Used

- NASM (`nasm -f elf64`)
- `ld` linker
- Linux syscalls:
  - `open`
  - `read`
  - `close`
  - `write`
- Buffer scanning using registers
- Loop and conditional jump instructions

---

## 4. Implementation Summary

The program:

1. Opens `sensor_log.txt` using syscall 2 (`open`)
2. Reads file contents into a buffer using syscall 0 (`read`)
3. Iterates byte-by-byte:
   - Increments counter whenever a `'\n'` is seen  
   - Correctly counts blank lines and trailing newline
4. Converts count to ASCII
5. Prints the message using syscall 1 (`write`)

---

## 5. Build / Run

```bash
nasm -f elf64 sensor_count.asm -o sensor_count.o
ld sensor_count.o -o sensor_count
echo -e "23\n45\n\n67\n" > sensor_log.txt
./sensor_count
```
