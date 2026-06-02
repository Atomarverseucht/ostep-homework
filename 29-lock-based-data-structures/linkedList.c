#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <assert.h>

// Struktur für einen Knoten mit eigenem Lock
typedef struct __node_t {
    int key;
    struct __node_t *next;
    pthread_mutex_t lock;
} node_t;

// Listenstruktur
typedef struct __list_t {
    node_t *head;
    pthread_mutex_t list_lock; // Schützt den Zugriff auf den Kopf der Liste
} list_t;

void List_Init(list_t *L) {
    L->head = NULL;
    pthread_mutex_init(&L->list_lock, NULL);
}

// Einfügen eines neuen Elements am Anfang
void List_Insert(list_t *L, int key) {
    node_t *new = malloc(sizeof(node_t));
    assert(new != NULL);
    new->key = key;
    pthread_mutex_init(&new->lock, NULL);

    pthread_mutex_lock(&L->list_lock);
    new->next = L->head;
    L->head = new;
    pthread_mutex_unlock(&L->list_lock);
}

int List_Lookup(list_t *L, int key) {
    node_t *curr = NULL;
    
    pthread_mutex_lock(&L->list_lock);
    curr = L->head;
    if (curr) {
        pthread_mutex_lock(&curr->lock); // Sperre den ersten Knoten
    }
    pthread_mutex_unlock(&L->list_lock);

    while (curr) {
        if (curr->key == key) {
            pthread_mutex_unlock(&curr->lock);
            return 0; // Gefunden
        }
        
        node_t *next_node = curr->next;
        if (next_node) {
            pthread_mutex_lock(&next_node->lock);
        }
        pthread_mutex_unlock(&curr->lock);
        curr = next_node;
    }
    return -1;
}

int main() {
    list_t mylist;
    List_Init(&mylist);

    // Liste füllen
    for (int i = 0; i < 10; i++) {
        List_Insert(&mylist, i);
    }

    // Suche testen
    if (List_Lookup(&mylist, 5) == 0) {
        printf("Schlüssel 5 gefunden.\n");
    } else {
        printf("Schlüssel 5 nicht gefunden.\n");
    }
    return 0;
}