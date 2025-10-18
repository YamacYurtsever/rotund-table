#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>

#define N 5
#define ROUNDS 10
#define LEFT(i)  (((i) + N - 1) % N)
#define RIGHT(i) (((i) + 1) % N)

enum state { THINKING, HUNGRY, EATING };

enum state states[N];
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
sem_t fork_sem[N];

void *philosopher(void *arg);
void take_forks(int i);
void put_forks(int i);
void test(int i);

int main(void) {
    pthread_t phil[N];

    for (int i = 0; i < N; i++) {
        states[i] = THINKING;
        sem_init(&fork_sem[i], 0, 0);
    }

    for (int i = 0; i < N; i++) {
        pthread_create(&phil[i], NULL, philosopher, (void *)(long)i);
    }

    for (int i = 0; i < N; i++) {
        pthread_join(phil[i], NULL);
    }

    return 0;
}

void *philosopher(void *arg) {
    int i = (int)(long)arg;
    for (int j = 0; j < ROUNDS; j++) {
        printf("Philosopher %d is THINKING\n", i);
        take_forks(i);
        printf("Philosopher %d is EATING\n", i);
        put_forks(i);
    }
    return NULL;
}

void take_forks(int i) {
    pthread_mutex_lock(&mutex);
    states[i] = HUNGRY;
    test(i);
    pthread_mutex_unlock(&mutex);
    sem_wait(&fork_sem[i]);
}

void put_forks(int i) {
    pthread_mutex_lock(&mutex);
    states[i] = THINKING;
    test(LEFT(i));
    test(RIGHT(i));
    pthread_mutex_unlock(&mutex);
}

void test(int i) {
    if (states[i] == HUNGRY && states[LEFT(i)] != EATING && states[RIGHT(i)] != EATING) {
        states[i] = EATING;
        sem_post(&fork_sem[i]);
    }
}
