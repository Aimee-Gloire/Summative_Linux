# Summative Linux Programming

This repository contains five question solutions for the Summative Project.

Folders:

- question1/ ELF analysis artifacts and report
- question2/ NASM assembly line counter
- question3/ C extension for Python
- question4/ Producer-consumer pthreads
- question5/ TCP client-server exam system

Each question has its own README with instructions on:  

- How to compile  
- How to run  
- What files are included  
- What the program does  

---

## Question Summaries

### **Q1 – ELF Reverse Engineering**

Used: `objdump`, `strace`, `gdb`  
Delivered: full disassembly, system call summary, control-flow explanation, memory access notes.

### **Q2 – Assembly**

NASM program that reads `sensor_log.txt`, counts all lines including empty ones, and prints  
**Total sensor readings: X**

### **Q3 – C Extension**

Implemented a Python C-extension with:

- min_temp
- max_temp
- avg_temp
- variance_temp
- count_readings  
Includes setup.py and test.py.

### **Q4 – Pthreads Simulation**

Simulated barista (producer) and waiter (consumer) using mutex + condition variables.

### **Q5 – TCP Client–Server**

Real-time exam system:

- Authentication  
- Question → Answer → Feedback  
- Active student list  
- Threaded server  
- Multiple clients allowed  

---

To run each task, open the corresponding folder and follow its README.
