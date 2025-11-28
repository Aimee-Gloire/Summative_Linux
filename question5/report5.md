# Question 5 — Real-time Exam Client–Server

Files:

- server.c
- client.c

Build:
gcc server.c -o server -lpthread
gcc client.c -o client

Run:

1) ./server
2) ./client   (in separate terminals, up to 4 concurrent clients)

Notes:

- The server authenticates from a small whitelist (edit `valid_users[]` as needed).
- Each client sends username, receives a question, submits an answer, receives immediate feedback and an active user list.
- Each client is handled in its own thread; active user list is protected by a mutex.
