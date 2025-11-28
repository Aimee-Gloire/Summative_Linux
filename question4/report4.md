# Question 4 — Producer-Consumer (Barista & Waiter)

Files:

- barista_waiter.c

Build:
gcc barista_waiter.c -o barista_waiter -lpthread

Run:
./barista_waiter

Synchronization Used:

pthread_mutex_t
pthread_cond_t
Barista waits when queue full
Waiter waits when queue empty
No race conditions (mutex protects queue_count)
