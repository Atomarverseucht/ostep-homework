#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>

long long int count = 0;
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

void countWithoutLock(int thread){
    count++;
    printf("Thread %d: %lld\n", thread, count);
    
}

void addAtomicCount(int thread){
    pthread_mutex_lock(&lock);
    countWithoutLock(thread);
    if(count >= (long long) 1000000) {
        printf("-------> reached: %lld\nThis is the version with Locks\n", count); 
        exit(0);
    }
    pthread_mutex_unlock(&lock);
}


void* secondThread(void* input){
    int thread = * (int*)input;
    while(1){
      addAtomicCount(thread);
    }
    return NULL;
}
void* thirdThread(void* input){
    countWithoutLock(* (int*)input);
    return NULL;
}

void v1(){
    pthread_t threads[100];
    for (int i = 0; i < 100; i++) {
        int* threadNumber = malloc(sizeof(int));
        *threadNumber = i;
        pthread_create(&threads[i], NULL, secondThread, threadNumber);
    }
    for (int i = 0; i < 100; i++) {
        pthread_join(threads[i], NULL);
    }
}

void v2(){
   pthread_t threads[100000];
    for (int i = 0; i < 100000; i++) {
        int* threadNumber = malloc(sizeof(int));
        *threadNumber = i;
        pthread_create(&threads[i], NULL, thirdThread, threadNumber);
    }
    for (int i = 0; i < 100000; i++) {
        pthread_join(threads[i], NULL);
    } 
    printf("-------> reached: %lld\n", count);
    printf("This is the example without locks\n");
}
int main(int argc, char* argv[]){
    v2();
}