#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#define N_PHIL 5
#define ROUNDS 3
#define LEFT(i)  (((i) + N_PHIL - 1) % N_PHIL)
#define RIGHT(i) (((i) + 1) % N_PHIL)

enum state { THINKING, HUNGRY, EATING };

enum state states[N_PHIL];
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t phil_mutex[N_PHIL];

void *philosopher(void *arg);
void think(int i);
void take_forks(int i);
void eat(int i);
void put_forks(int i);
void test(int i);

int main(void) {
    pthread_t phil[N_PHIL];

    for (int i = 0; i < N_PHIL; i++) {
        states[i] = THINKING;
        pthread_mutex_init(&phil_mutex[i], NULL);
        pthread_mutex_lock(&phil_mutex[i]);
    }

    for (int i = 0; i < N_PHIL; i++) {
        pthread_create(&phil[i], NULL, philosopher, (void *)(long)i);
    }

    for (int i = 0; i < N_PHIL; i++) {
        pthread_join(phil[i], NULL);
    }

    return 0;
}

void *philosopher(void *arg) {
    int i = (int)(long)arg;
    for (int j = 0; j < ROUNDS; j++) {
        think(i);
        take_forks(i);
        eat(i);
        put_forks(i);
    }
    return NULL;
}

void think(int i) {
    printf("Philosopher %d is THINKING\n", i);
    usleep(rand() % 500000);
}

void take_forks(int i) {
    pthread_mutex_lock(&mutex);
    states[i] = HUNGRY;
    test(i);
    pthread_mutex_unlock(&mutex);
    pthread_mutex_lock(&phil_mutex[i]);
}

void eat(int i) {
    printf("Philosopher %d is EATING\n", i);
    usleep(rand() % 500000);
}

void put_forks(int i) {
    pthread_mutex_lock(&mutex);
    states[i] = THINKING;
    test(LEFT(i));
    test(RIGHT(i));
    pthread_mutex_unlock(&mutex);
}

void test(int i) {
    if (states[i] == HUNGRY &&
        states[LEFT(i)] != EATING &&
        states[RIGHT(i)] != EATING) {
        states[i] = EATING;
        pthread_mutex_unlock(&phil_mutex[i]);
    }
}
