
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>
#include <unistd.h>

#define MAX_CPUS 64

typedef struct __counter_t {
    int global;
    pthread_mutex_t glock;
    int local[MAX_CPUS];
    pthread_mutex_t llock[MAX_CPUS];
    int threshold;
} sloppy_counter_t;

void init(sloppy_counter_t *c, int threshold) {
    c->threshold = threshold;
    c->global = 0;
    pthread_mutex_init(&c->glock, NULL);
    for (int i = 0; i < MAX_CPUS; i++) {
        c->local[i] = 0;
        pthread_mutex_init(&c->llock[i], NULL);
    }
}

void update(sloppy_counter_t *c, int threadID, int amt) {
    int cpu = threadID % sysconf(_SC_NPROCESSORS_ONLN);
    pthread_mutex_lock(&c->llock[cpu]);
    c->local[cpu] += amt;
    if (c->local[cpu] >= c->threshold) {
        pthread_mutex_lock(&c->glock);
        c->global += c->local[cpu];
        pthread_mutex_unlock(&c->glock);
        c->local[cpu] = 0;
    }
    pthread_mutex_unlock(&c->llock[cpu]);
}

int get(sloppy_counter_t *c) {
    pthread_mutex_lock(&c->glock);
    int val = c->global;
    pthread_mutex_unlock(&c->glock);
    return val;
}

typedef struct {
    sloppy_counter_t *c;
    int id;
} thread_args_t;

void *worker(void *arg) {
    thread_args_t *args = (thread_args_t *)arg;
    for (int i = 0; i < 1000000; i++)
        update(args->c, args->id, 1);
    return NULL;
}

int main(int argc, char *argv[]) {
    int num_threads = (argc > 1) ? atoi(argv[2]) : 4;
    int S = (argc > 2) ? atoi(argv[7]) : 1024;
    
    sloppy_counter_t c;
    init(&c, S);

    pthread_t threads[num_threads];
    thread_args_t args[num_threads];

    struct timeval start, end;
    gettimeofday(&start, NULL);

    for (int i = 0; i < num_threads; i++) {
        args[i].c = &c;
        args[i].id = i;
        pthread_create(&threads[i], NULL, worker, &args[i]);
    }
    for (int i = 0; i < num_threads; i++)
        pthread_join(threads[i], NULL);

    gettimeofday(&end, NULL);
    double elapsed = (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) / 1000000.0;

    printf("Threads: %d, Schwellenwert S: %d\n", num_threads, S);
    printf("Globaler (approximativer) Wert: %d\n", get(&c));
    printf("Benötigte Zeit: %.4f Sekunden\n", elapsed);
    return 0;
}