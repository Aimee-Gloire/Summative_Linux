# Question 5 — Real-time Exam Client–Server

## 1. Overview

This project implements a real-time exam platform with:

### SERVER

- Handles up to 4 concurrent students
- Authenticates username
- Sends exam question
- Receives answer
- Sends feedback
- Shares active student list
- Ends session gracefully

### CLIENT

- Connects to server
- Sends username
- Receives question
- Sends answer
- Receives feedback and final messages

Both programs use TCP sockets and threads.

---

## 2. Features Implemented

- Username authentication  
- Multi-threaded server  
- Thread-safe active-student list  
- Instant question delivery  
- Instant feedback  
- Clean session termination  
- Multiple clients supported simultaneously

---

## 3. How to Compile and Run

### Compile

```bash
gcc server.c -o server -lpthread
gcc client.c -o client
```

Run:

```bash
./server
./client   (in separate terminals, up to 4 concurrent clients)
```
