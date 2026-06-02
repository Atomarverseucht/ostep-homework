#include <stdio.h>
#include <pthread.h>
#include <sys/time.h>

typedef struct {
    int value;
    pthread_mutex_t lock;
} counter_t;

void init(counter_t *c) {
    c->value = 0;
    pthread_mutex_init(&c->lock, NULL);
}

void increment(counter_t *c) {
    pthread_mutex_lock(&c->lock);
    c->value++;
    pthread_mutex_unlock(&c->lock);
}

void *worker(void *arg) {
    counter_t *c = (counter_t *) arg;
    for (int i = 0; i < 1000000; i++) increment(c);
    return NULL;
}

int main() {
    counter_t c;
    init(&c);
    pthread_t p1, p2;
    pthread_create(&p1, NULL, worker, &c);
    pthread_create(&p2, NULL, worker, &c);
    pthread_join(p1, NULL);
    pthread_join(p2, NULL);
    printf("Finaler Wert: %d\n", c.value);
    return 0;
}