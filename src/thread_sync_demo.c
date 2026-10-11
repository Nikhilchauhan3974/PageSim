#include <stdio.h>
#include <pthread.h>

#define ITERATIONS 100000

int shared_counter = 0;
pthread_mutex_t lock;

void *increment_counter(void *arg)
{
    (void)arg;

    for (int i = 0; i < ITERATIONS; i++) {
        pthread_mutex_lock(&lock);
        shared_counter++;
        pthread_mutex_unlock(&lock);
    }

    return NULL;
}

int main(void)
{
    pthread_t thread1, thread2;

    printf("\n=== THREAD SYNCHRONIZATION DEMO ===\n");

    if (pthread_mutex_init(&lock, NULL) != 0) {
        fprintf(stderr, "Mutex initialization failed.\n");
        return 1;
    }

    if (pthread_create(&thread1, NULL, increment_counter, NULL) != 0) {
        fprintf(stderr, "Thread 1 creation failed.\n");
        pthread_mutex_destroy(&lock);
        return 1;
    }

    if (pthread_create(&thread2, NULL, increment_counter, NULL) != 0) {
        fprintf(stderr, "Thread 2 creation failed.\n");
        pthread_join(thread1, NULL);
        pthread_mutex_destroy(&lock);
        return 1;
    }

    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);

    printf("Thread 1 increments: %d\n", ITERATIONS);
    printf("Thread 2 increments: %d\n", ITERATIONS);
    printf("Final shared counter: %d\n", shared_counter);
    printf("Expected counter: %d\n", 2 * ITERATIONS);

    pthread_mutex_destroy(&lock);

    printf("Synchronization demo completed.\n");
    return 0;
}
