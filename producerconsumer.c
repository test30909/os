#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define CAPACITY 5 // Capacity of the buffer

// Buffer and synchronization variables
int buffer[CAPACITY];
int in = 0, out = 0; // Indices for the producer and consumer
sem_t empty; // Semaphore to track empty slots
sem_t full;  // Semaphore to track full slots
sem_t pmutex; // Mutex for producer access
sem_t cmutex; // Mutex for consumer access

// Function for producer
void *producer(void *arg) {
    int producedItem;
    while (1) {
        // Produce the data to be put into buffer
        producedItem = rand() % 100; // Produce a random item

        sem_wait(&empty); // Wait if the buffer is full
        sem_wait(&pmutex); // Lock the buffer for producer

        // Critical section
        buffer[in] = producedItem; // Put the item into the buffer
        printf("Produced: %d at index %d\n", producedItem, in);
        in = (in + 1) % CAPACITY; // Update the index for the next item

        sem_post(&pmutex); // Unlock the buffer
        sem_post(&full); // Signal that there's a new item in the buffer

        // Simulate production time
        sleep(rand() % 2);
    }
}

// Function for consumer
void *consumer(void *arg) {
    int consumedItem;
    while (1) {
        sem_wait(&full); // Wait if the buffer is empty
        sem_wait(&cmutex); // Lock the buffer for consumer

        // Critical section
        consumedItem = buffer[out]; // Get the item from the buffer
        printf("Consumed: %d from index %d\n", consumedItem, out);
        out = (out + 1) % CAPACITY; // Update the index for the next item

        sem_post(&cmutex); // Unlock the buffer
        sem_post(&empty); // Signal that there's an empty slot in the buffer

        // Simulate consumption time
        sleep(rand() % 2);
    }
}

int main() {
    pthread_t prod, cons;

    // Initialize semaphores
    sem_init(&empty, 0, CAPACITY); // Initialize empty to capacity
    sem_init(&full, 0, 0); // Initialize full to 0
    sem_init(&pmutex, 0, 1); // Initialize producer mutex
    sem_init(&cmutex, 0, 1); // Initialize consumer mutex

    // Create producer and consumer threads
    pthread_create(&prod, NULL, producer, NULL);
    pthread_create(&cons, NULL, consumer, NULL);

    // Wait for threads to finish (they won't in this infinite loop)
    pthread_join(prod, NULL);
    pthread_join(cons, NULL);

    // Clean up semaphores (will not be reached in this case)
    sem_destroy(&empty);
    sem_destroy(&full);
    sem_destroy(&pmutex);
    sem_destroy(&cmutex);

    return 0;
}
