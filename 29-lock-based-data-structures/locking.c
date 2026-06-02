#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>

// Basis-Konfiguration
#define NUM_BUCKETS 101
#define NUM_THREADS 4
#define INSERTS_PER_THREAD 100000

// --- Datenstrukturen ---

typedef struct __node_t {
    int key;
    struct __node_t *next;
} node_t;

typedef struct __list_t {
    node_t *head;
    pthread_mutex_t lock; // Wird für die feingranulare Strategie genutzt
} list_t;

// Einfache Liste initialisieren
void List_Init(list_t *L) {
    L->head = NULL;
    pthread_mutex_init(&L->lock, NULL);
}

// Einfügen in die Liste (mit internem Lock für feingranularen Ansatz)
void List_Insert_InternalLock(list_t *L, int key) {
    pthread_mutex_lock(&L->lock);
    node_t *new = malloc(sizeof(node_t));
    new->key = key;
    new->next = L->head;
    L->head = new;
    pthread_mutex_unlock(&L->lock);
}

// Einfügen in die Liste (OHNE internes Lock für grobgranularen Ansatz)
void List_Insert_NoLock(list_t *L, int key) {
    node_t *new = malloc(sizeof(node_t));
    new->key = key;
    new->next = L->head;
    L->head = new;
}

// --- Strategie 1: Ein großes globales Lock ---
typedef struct {
    list_t lists[NUM_BUCKETS];
    pthread_mutex_t global_lock;
} hash_table_global_t;

void Hash_Global_Init(hash_table_global_t *H) {
    for (int i = 0; i < NUM_BUCKETS; i++) List_Init(&H->lists[i]);
    pthread_mutex_init(&H->global_lock, NULL);
}

void Hash_Global_Insert(hash_table_global_t *H, int key) {
    pthread_mutex_lock(&H->global_lock);
    List_Insert_NoLock(&H->lists[key % NUM_BUCKETS], key);
    pthread_mutex_unlock(&H->global_lock);
}

// --- Strategie 2: Ein Lock pro Bucket (Interessante Strategie) ---
typedef struct {
    list_t lists[NUM_BUCKETS];
} hash_table_bucket_t;

void Hash_Bucket_Init(hash_table_bucket_t *H) {
    for (int i = 0; i < NUM_BUCKETS; i++) List_Init(&H->lists[i]);
}

void Hash_Bucket_Insert(hash_table_bucket_t *H, int key) {
    // Nutzt das Lock innerhalb der Liste des jeweiligen Buckets
    List_Insert_InternalLock(&H->lists[key % NUM_BUCKETS], key);
}

// --- Thread-Worker und Zeitmessung ---

void *worker_global(void *arg) {
    hash_table_global_t *H = (hash_table_global_t *)arg;
    for (int i = 0; i < INSERTS_PER_THREAD; i++) Hash_Global_Insert(H, i);
    return NULL;
}

void *worker_bucket(void *arg) {
    hash_table_bucket_t *H = (hash_table_bucket_t *)arg;
    for (int i = 0; i < INSERTS_PER_THREAD; i++) Hash_Bucket_Insert(H, i);
    return NULL;
}

double get_time() {
    struct timeval t;
    gettimeofday(&t, NULL);
    return (double)t.tv_sec + (double)t.tv_usec / 1000000.0;
}

int main() {
    pthread_t threads[NUM_THREADS];
    double start, end;

    // Test 1: Globales Lock (Straightforward)
    hash_table_global_t Hg;
    Hash_Global_Init(&Hg);
    printf("Starte Test: Globales Lock mit %d Threads...\n", NUM_THREADS);
    start = get_time();
    for (int i = 0; i < NUM_THREADS; i++) pthread_create(&threads[i], NULL, worker_global, &Hg);
    for (int i = 0; i < NUM_THREADS; i++) pthread_join(threads[i], NULL);
    end = get_time();
    double time_global = end - start;
    printf("Zeit (Global Lock): %.4f Sekunden\n\n", time_global);

    // Test 2: Bucket Locks (Interessante Strategie)
    hash_table_bucket_t Hb;
    Hash_Bucket_Init(&Hb);
    printf("Starte Test: Bucket-Level Locks mit %d Threads...\n", NUM_THREADS);
    start = get_time();
    for (int i = 0; i < NUM_THREADS; i++) pthread_create(&threads[i], NULL, worker_bucket, &Hb);
    for (int i = 0; i < NUM_THREADS; i++) pthread_join(threads[i], NULL);
    end = get_time();
    double time_bucket = end - start;
    printf("Zeit (Bucket Locks): %.4f Sekunden\n\n", time_bucket);
    return 0;
}