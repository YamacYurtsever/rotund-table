#include <stdio.h>
#include <pthread.h>

pthread_mutex_t lock_A = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t lock_B = PTHREAD_MUTEX_INITIALIZER;

void *thread_A(void *arg);
void *thread_B(void *arg);

int main(void) {
    pthread_t a;
    pthread_t b;

    pthread_create(&a, NULL, thread_A, NULL);
    pthread_create(&b, NULL, thread_B, NULL);

    pthread_join(a, NULL);
    pthread_join(b, NULL);
}

void *thread_A(void *arg) {
    pthread_mutex_lock(&lock_A);
    printf("Thread A locked A\n");
    pthread_mutex_lock(&lock_B);
    printf("Thread A locked B\n");
    pthread_mutex_unlock(&lock_B);
    pthread_mutex_unlock(&lock_A);
    return NULL;
}

void *thread_B(void *arg) {
    pthread_mutex_lock(&lock_B);
    printf("Thread B locked B\n");
    pthread_mutex_lock(&lock_A);
    printf("Thread B locked A\n");
    pthread_mutex_unlock(&lock_A);
    pthread_mutex_unlock(&lock_B);
    return NULL;
}
