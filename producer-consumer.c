#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>

#define BUFFER_SIZE 3
#define ROUNDS 10
#define EMPTY -1
#define FULL 1

int buffer[BUFFER_SIZE];
int in = 0;
int out = 0;

sem_t mutex;
sem_t empty;
sem_t full;

void *producer(void *arg);
void *consumer(void *arg);

int main(void) {
    sem_init(&mutex, 0, 1);
    sem_init(&empty, 0, BUFFER_SIZE);
    sem_init(&full, 0, 0);

    for (int i = 0; i < BUFFER_SIZE; i++) {
        buffer[i] = EMPTY;
    }

    pthread_t prod;
    pthread_t cons;

    pthread_create(&prod, NULL, producer, NULL);
    pthread_create(&cons, NULL, consumer, NULL);

    pthread_join(prod, NULL);
    pthread_join(cons, NULL);

    return 0;
}

void *producer(void *arg) {
    for (int i = 0; i < ROUNDS; i++) {
        sem_wait(&empty);
        sem_wait(&mutex);

        buffer[in] = i;
        in = (in + 1) % BUFFER_SIZE;
        printf("Produced %d\n", i);

        sem_post(&mutex);
        sem_post(&full);
    }
    return NULL;
}

void *consumer(void *arg) {
    for (int i = 0; i < ROUNDS; i++) {
        sem_wait(&full);
        sem_wait(&mutex);

        int item = buffer[out];
        buffer[out] = EMPTY;
        out = (out + 1) % BUFFER_SIZE;
        printf("Consumed %d\n", item);

        sem_post(&mutex);
        sem_post(&empty);
    }
    return NULL;
}
