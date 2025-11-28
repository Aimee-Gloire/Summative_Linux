# Question 4 — Producer-Consumer (Barista & Waiter)

Files:

- barista_waiter.c

Build:
gcc barista_waiter.c -o barista_waiter -lpthread

Run:
./barista_waiter

Behavior:

- Barista (producer) prepares a drink every 4s; waiter (consumer) serves every 3s.
- Shared queue capacity: 8.
- Proper synchronization: mutex + condvars (`queue_not_full`, `queue_not_empty`).
- Console output shows queue size, waits on full/empty conditions.
