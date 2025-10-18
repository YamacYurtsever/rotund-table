#include <stdio.h>
#include <pthread.h>

#define N_THREADS 100
#define AMOUNT 10000

int count = 0;

void *thread(void *arg);

int main(void) {
    pthread_t threads[N_THREADS];

    for (int i = 0; i < N_THREADS; i++) {
        pthread_create(&threads[i], NULL, thread, NULL);
    }

    for (int i = 0; i < N_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    printf("%d\n", count);
    return 0;
}

void *thread(void *arg) {
    for (int i = 0; i < AMOUNT; i++) {
        count++;
    }
    return NULL;
}
