#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

#define N_READERS 5
#define N_WRITERS 2
#define ROUNDS 5

int buffer = -1;
int read_count = 0;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t rw_mutex = PTHREAD_MUTEX_INITIALIZER;

void *reader(void *arg);
void *writer(void *arg);

int main(void) {
    pthread_t readers[N_READERS];
    pthread_t writers[N_WRITERS];

    for (int i = 0; i < N_READERS; i++) {
        pthread_create(&readers[i], NULL, reader, (void *)(long)i);
    }

    for (int i = 0; i < N_WRITERS; i++) {
        pthread_create(&writers[i], NULL, writer, (void *)(long)i);
    }

    for (int i = 0; i < N_READERS; i++) {
        pthread_join(readers[i], NULL);
    }

    for (int i = 0; i < N_WRITERS; i++) {
        pthread_join(writers[i], NULL);
    }

    return 0;
}

void *reader(void *arg) {
    int id = (int)(long)arg;
    for (int i = 0; i < ROUNDS; i++) {
        pthread_mutex_lock(&mutex);
        read_count++;
        if (read_count == 1)
            pthread_mutex_lock(&rw_mutex);
        pthread_mutex_unlock(&mutex);

        printf("Reader %d reads buffer: %d\n", id, buffer);

        pthread_mutex_lock(&mutex);
        read_count--;
        if (read_count == 0)
            pthread_mutex_unlock(&rw_mutex);
        pthread_mutex_unlock(&mutex);

        usleep(rand() % 500000);
    }
    return NULL;
}

void *writer(void *arg) {
    int id = (int)(long)arg;
    for (int i = 0; i < ROUNDS; i++) {
        pthread_mutex_lock(&rw_mutex);

        buffer = i;
        printf("Writer %d writes buffer: %d\n", id, buffer);

        pthread_mutex_unlock(&rw_mutex);

        usleep(rand() % 500000);
    }
    return NULL;
}
