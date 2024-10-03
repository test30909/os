#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

sem_t mutex;  // Semaphore for mutual exclusion
sem_t write;  // Semaphore to control access to the shared resource
int reader_count = 0;  // Keeps track of the number of readers

// Function for readers
void *reader(void *arg) {
    int id = *((int *)arg);  // Get the reader ID

    while (1) {
        // Reader tries to access the shared resource
        sem_wait(&mutex);  // Lock the access to modify reader_count
//critical
        reader_count++;
        if (reader_count == 1) {
            sem_wait(&write);  // First reader locks the resource for writing
        }
//critical
        sem_post(&mutex);  // Release the mutex so other readers can come in

        // Reading section
        printf("Reader %d is reading\n", id);
        sleep(1);  // Simulate reading time

        // Reader leaves
        sem_wait(&mutex);  // Lock the access to modify reader_count

        reader_count--;
        if (reader_count == 0) {
            sem_post(&write);  // Last reader unlocks the resource for writing
        }
        sem_post(&mutex);  // Release the mutex

        // Simulate some processing after reading
        sleep(1);
    }
    return NULL;
}

// Function for writers
void *writer(void *arg) {
    int id = *((int *)arg);  // Get the writer ID

    while (1) {
        // Writer tries to access the shared resource
        sem_wait(&write);  // Lock the resource for writing

        // Writing section
        printf("Writer %d is writing\n", id);
        sleep(2);  // Simulate writing time

        // Writer leaves
        sem_post(&write);  // Unlock the resource

        // Simulate some processing after writing
        sleep(2);
    }
    return NULL;
}

int main() {
    pthread_t readers[5], writers[3];  // Create reader and writer threads
    int reader_ids[5], writer_ids[3];

    // Initialize the semaphores
    sem_init(&mutex, 0, 1);  // Initialize mutex to 1 (binary semaphore)
    sem_init(&write, 0, 1);  // Initialize write semaphore to 1 (binary semaphore)

    // Create 5 readers
    for (int i = 0; i < 5; i++) {
        reader_ids[i] = i + 1;
        pthread_create(&readers[i], NULL, reader, &reader_ids[i]);
    }

    // Create 3 writers
    for (int i = 0; i < 3; i++) {
        writer_ids[i] = i + 1;
        pthread_create(&writers[i], NULL, writer, &writer_ids[i]);
    }

    // Join the threads (although they will run indefinitely in this case)
    for (int i = 0; i < 5; i++) {
        pthread_join(readers[i], NULL);
    }
    for (int i = 0; i < 3; i++) {
        pthread_join(writers[i], NULL);
    }

    // Destroy semaphores (although this won't be reached due to the infinite loop)
    sem_destroy(&mutex);
    sem_destroy(&write);

    return 0;
}
