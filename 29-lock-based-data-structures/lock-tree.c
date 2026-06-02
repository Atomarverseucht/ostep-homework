#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

// Data-structure -> Tree
typedef struct element_t {
    int value;
    struct element_t* child[2];
    pthread_mutex_t* lock;
    pthread_cond_t* condition;
    int used;
} element;

element* head;

void lockElement(element* active){
    pthread_mutex_lock(active->lock);
    while(active->used != 0) 
        pthread_cond_wait(active->condition, active->lock);
    active->used = 1;
}

void unlockElement(element* active){
    active->used = 0;
    pthread_cond_signal(active->condition);
    pthread_mutex_unlock(active->lock);
}

element* newElement(int value){
    element* out = malloc(sizeof(element));
    out->value = value;
    out->used = 0;
    out->child[0] = NULL;
    out->child[1] = NULL;
    out->lock = malloc(sizeof(pthread_mutex_t));
    out->condition = malloc(sizeof(pthread_cond_t));
    if (out->condition == NULL || out->lock == NULL) {
        perror("Speicherallokation fehlgeschlagen");
        exit(-1);
    }

    pthread_mutex_init(out->lock, NULL);
    if (pthread_cond_init(out->condition, NULL) != 0) {
        perror("Cond-Initialisierung fehlgeschlagen");
        exit(-1);
    }
    return out;
}

int countElements(element* active){
    int count = 1;
    lockElement(active);
    if(active->child[0] != NULL) count += countElements(active->child[0]);
    if(active->child[1] != NULL) count += countElements(active->child[1]);
    unlockElement(active);
    return count;
}

void freeTree(element* active){
    if(active == NULL) return;
    freeTree(active->child[0]);
    freeTree(active->child[1]);
    free(active->lock);
    free(active->condition);
    free(active);
}

void addElement(int value){
    element* activeElem = head;
    element* lastElem = head;
    int isHead = 1;
    while(activeElem != NULL){
        lockElement(activeElem);
        if (!isHead) unlockElement(lastElem);
        isHead = 0;
        lastElem = activeElem;
        activeElem = lastElem->child[(value < lastElem->value)?1:0];
    }
    lastElem->child[(value < lastElem->value)?1:0] = newElement(value);
    unlockElement(lastElem);
}

void printTree(element* active){
    printf("%04d: ", active->value);
    if(active->child[0] != NULL) printf("%04d, ", active->child[0]->value);
    else printf("----, ");
    if(active->child[1] != NULL) printf("%04d", active->child[1]->value);
    else printf("----");
    printf("\n");
    if(active->child[0] != NULL) printTree(active->child[0]);
    if(active->child[1] != NULL) printTree(active->child[1]);
}

/// ANWENDUNG

void* worker(void* args){
    for(int i = 0; i < 10; i++){
        addElement(rand() % 100000);
    }
    return NULL;
}

int main(int argc, char *argv[]) {
    int threadCount = 1000;
    pthread_t thread[threadCount];
    head = newElement(11);
    for(int i = 0; i < threadCount; i++){
        pthread_create(&thread[i], NULL, worker, NULL);
    }
    for(int i = 0; i < threadCount; i++){
        pthread_join(thread[i], NULL);
    }

    printTree(head);
    printf("Count: %d\n", countElements(head));
    freeTree(head);
}