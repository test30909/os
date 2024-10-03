#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define N 5  // Number of philosophers

sem_t forks[N];  // One semaphore per fork
pthread_t philosophers[N];  // One thread per philosopher
int ids[N];  // Philosopher IDs

void *philosopher(void *arg);
void pick_up_forks(int philosopher_id);
void put_down_forks(int philosopher_id);
void think(int philosopher_id);
void eat(int philosopher_id);

int main() {
    // Initialize semaphores (forks)
    for (int i = 0; i < N; i++) {
        sem_init(&forks[i], 0, 1);  // Initialize each fork semaphore to 1 (available)
        ids[i] = i;  // Assign each philosopher a unique ID (0 to N-1)
    }

    // Create philosopher threads
    for (int i = 0; i < N; i++) {
        pthread_create(&philosophers[i], NULL, philosopher, &ids[i]);
    }

    // Join philosopher threads (wait for all threads to finish, though they run indefinitely)
    for (int i = 0; i < N; i++) {
        pthread_join(philosophers[i], NULL);
    }

    // Clean up: Destroy the semaphores
    for (int i = 0; i < N; i++) {
        sem_destroy(&forks[i]);
    }

    return 0;
}

void *philosopher(void *arg) {
    int id = *((int *)arg);  // Get philosopher ID

    while (1) {
        think(id);  // Philosopher is thinking
        pick_up_forks(id);  // Philosopher tries to pick up forks to eat
        eat(id);  // Philosopher is eating
        put_down_forks(id);  // Philosopher puts down forks after eating
    }
}

void pick_up_forks(int philosopher_id) {
    // Pick up the left and right forks (with circular array of forks)
    int left_fork = philosopher_id;  // Left fork is the same index as philosopher
    int right_fork = (philosopher_id + 1) % N;  // Right fork is the next one (circular)

    // Pick up the lower-numbered fork first to avoid deadlock
    if (philosopher_id % 2 == 0) {
        sem_wait(&forks[left_fork]);  // Wait for left fork
        sem_wait(&forks[right_fork]);  // Wait for right fork
    } else {
        sem_wait(&forks[right_fork]);  // Wait for right fork
        sem_wait(&forks[left_fork]);  // Wait for left fork
    }

    printf("Philosopher %d picked up forks %d and %d\n", philosopher_id, left_fork, right_fork);
}

void put_down_forks(int philosopher_id) {
    int left_fork = philosopher_id;
    int right_fork = (philosopher_id + 1) % N;

    // Put down both forks
    sem_post(&forks[left_fork]);  // Release left fork
    sem_post(&forks[right_fork]);  // Release right fork

    printf("Philosopher %d put down forks %d and %d\n", philosopher_id, left_fork, right_fork);
}

void think(int philosopher_id) {
    printf("Philosopher %d is thinking\n", philosopher_id);
    sleep(rand() % 3);  // Simulate thinking time
}

void eat(int philosopher_id) {
    printf("Philosopher %d is eating\n", philosopher_id);
    sleep(rand() % 3);  // Simulate eating time
}
