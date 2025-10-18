#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>

#define N_READERS_WRITERS 2
#define ROUNDS 5

int buffer = -1;
int read_count = 0;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t rw_mutex = PTHREAD_MUTEX_INITIALIZER;

void *reader(void *arg);
void *writer(void *arg);

int main(void) {
    pthread_t readers[N_READERS_WRITERS];
    pthread_t writers[N_READERS_WRITERS];

    for (int i = 0; i < N_READERS_WRITERS; i++) {
        pthread_create(&readers[i], NULL, reader, (void *)(long)i);
        pthread_create(&writers[i], NULL, writer, (void *)(long)i);
    }

    for (int i = 0; i < N_READERS_WRITERS; i++) {
        pthread_join(readers[i], NULL);
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
    }
    return NULL;
}
