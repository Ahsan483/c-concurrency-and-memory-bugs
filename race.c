/*
 * race.c - a data race and its fix
 *
 * 4 threads each add 1,000,000 to a shared counter.
 *   make race      -> no lock, the total comes out wrong
 *   make race_fix  -> mutex around the update, always 4000000
 */
#include <pthread.h>
#include <stdio.h>

#define THREADS 4
#define ADDS 1000000

static long counter;
#ifdef FIXED
static pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
#endif

static void *work(void *arg)
{
    (void)arg;
    for (int i = 0; i < ADDS; i++) {
#ifdef FIXED
        pthread_mutex_lock(&lock);
        counter++;
        pthread_mutex_unlock(&lock);
#else
        counter++;                       /* read-modify-write: not atomic */
#endif
    }
    return NULL;
}

int main(void)
{
    pthread_t t[THREADS];
    for (int i = 0; i < THREADS; i++) pthread_create(&t[i], NULL, work, NULL);
    for (int i = 0; i < THREADS; i++) pthread_join(t[i], NULL);
    printf("expected %d, got %ld%s\n", THREADS * ADDS, counter,
           counter == THREADS * ADDS ? "  (correct)" : "  <- lost updates");
    return 0;
}
