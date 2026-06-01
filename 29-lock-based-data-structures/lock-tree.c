#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct element_ {
    int value;
    struct element_* child[2];
    pthread_mutex_t* lock;
    pthread_cond_t* condition;
    int used;
} element;

element* head;

element* newElement(int value){
    element* out = malloc(sizeof(element));
    out->value = value;
    out->used = 0;
    out->child[0] = malloc(sizeof(element));
    out->child[1] = malloc(sizeof(element));
    out->lock = malloc(sizeof(pthread_mutex_t));
    out->condition = malloc(sizeof(pthread_cond_t));
    if (out->condition == NULL || out->lock == NULL || out->child[0] == NULL || out->child[1] == NULL) {
        perror("Speicherallokation fehlgeschlagen");
        exit(1);
    }

    pthread_mutex_init(out->lock, NULL);
    if (pthread_cond_init(out->condition, NULL) != 0) {
        perror("Cond-Initialisierung fehlgeschlagen");
        exit(1);
    }
    return out;
}

void freeTree(element* active){
    if(active->child[0] != NULL) freeTree(active->child[0]);
    if(active->child[1] != NULL) freeTree(active->child[1]);
    free(active->lock);
    free(active->condition);
    free(active);
}

void addElement(int value){
    element* activeElem = head;
    element* lastElem = head;
    int isHead = 1;
    while(activeElem != NULL){
        pthread_mutex_lock(activeElem->lock);
        if (!isHead) pthread_mutex_unlock(lastElem->lock);
        isHead = 0;
        lastElem = activeElem;
        activeElem = lastElem->child[(value < lastElem->value)?1:0];
    }
    lastElem->child[(value < lastElem->value)?1:0] = newElement(value);
    pthread_mutex_unlock(lastElem->lock);
}

void printTree(element* active){
    printf("%d: %d, %d\n", active->value, active->child[0]->value, active->child[1]->value);
    if(active->child[0] != NULL) printTree(active->child[0]);
    if(active->child[1] != NULL) printTree(active->child[1]);
}

void* worker(void* args){
    return NULL;
}

int main(int argc, char *argv[]) {
    head = newElement(11);
    int values[] = {10,14,9,12,8,27};
    for(int i = 0; i < 6; i++){
        addElement(values[i]);
    }
    printTree(head);
    freeTree(head);
}