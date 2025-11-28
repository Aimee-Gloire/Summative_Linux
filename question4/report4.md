# Question 4 — Producer/Consumer: Barista & Waiter (Pthreads)

## 1. Overview

This simulation implements two threads:

- A **Barista** (producer) who prepares a drink every 4 seconds
- A **Waiter** (consumer) who serves a drink every 3 seconds

Both interact through a shared queue of max size 8.  
Proper synchronization ensures:

- Barista waits when queue is full
- Waiter waits when queue is empty
- No race conditions
- Clean, predictable output

---

## 2. Tools and Concepts Used

- pthreads
- mutex locking (`pthread_mutex_t`)
- condition variables (`pthread_cond_t`)
- bounded queue
- thread synchronization
- sleep timing (`sleep()`)

---

## 3. Implementation Summary

The program includes:

- Shared variable `queue_count`
- Mutex `lock`
- Two condition variables:
  - `queue_not_full`
  - `queue_not_empty`

Producer (barista):

- Sleeps 4 seconds  
- Checks queue fullness  
- Adds drink  
- Signals waiter  

Consumer (waiter):

- Waits for available drink  
- Serves (removes) drink  
- Sleeps 3 seconds  
- Signals barista  

---

## 4. How to Compile and Run

```bash
gcc barista_waiter.c -o barista_waiter -lpthread
./barista_waiter
```
