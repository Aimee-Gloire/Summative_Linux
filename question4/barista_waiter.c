#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

// ===============================================================
// Shared queue settings
// ===============================================================
#define MAX_QUEUE 8

int queue_count = 0;                // how many drinks in queue
pthread_mutex_t lock;               // mutex to protect queue
pthread_cond_t queue_not_full;      // barista waits here when queue is full
pthread_cond_t queue_not_empty;     // waiter waits here when queue is empty

// ===============================================================
// Producer: Barista Thread
// ===============================================================
void* barista(void* arg) {
    while (1) {
        sleep(4); // takes 4 seconds to prepare a drink

        pthread_mutex_lock(&lock);

        // If queue is full → wait
        while (queue_count == MAX_QUEUE) {
            printf("Barista: Queue full (%d). Waiting...\n", queue_count);
            pthread_cond_wait(&queue_not_full, &lock);
        }

        // Add drink to queue
        queue_count++;
        printf("Barista: Prepared a drink. Queue size: %d\n", queue_count);

        // Signal waiter that new drink is available
        pthread_cond_signal(&queue_not_empty);

        pthread_mutex_unlock(&lock);
    }
    return NULL;
}

// ===============================================================
// Consumer: Waiter Thread
// ===============================================================
void* waiter(void* arg) {
    while (1) {
        pthread_mutex_lock(&lock);

        // If queue is empty → wait
        while (queue_count == 0) {
            printf("Waiter: Queue empty. Waiting...\n");
            pthread_cond_wait(&queue_not_empty, &lock);
        }

        // Serve a drink
        queue_count--;
        printf("Waiter: Served a drink. Queue size: %d\n", queue_count);

        // Now space is available → signal barista
        pthread_cond_signal(&queue_not_full);

        pthread_mutex_unlock(&lock);

        sleep(3); // takes 3 seconds to serve a drink
    }
    return NULL;
}

// ===============================================================
// Main function
// ===============================================================
int main() {
    pthread_t barista_thread, waiter_thread;

    // Initialize synchronization tools
    pthread_mutex_init(&lock, NULL);
    pthread_cond_init(&queue_not_full, NULL);
    pthread_cond_init(&queue_not_empty, NULL);

    // Create threads
    pthread_create(&barista_thread, NULL, barista, NULL);
    pthread_create(&waiter_thread, NULL, waiter, NULL);

    // Keep main alive
    pthread_join(barista_thread, NULL);
    pthread_join(waiter_thread, NULL);

    return 0;
}
