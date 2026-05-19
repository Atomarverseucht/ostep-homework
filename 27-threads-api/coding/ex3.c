#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define arrLen 100000
int arr[arrLen];


void* worker(void* input){
    int* odd =  (int*) input;
    int count = 0;
    for(int i = 0; i < arrLen; i++){
        if(arr[i] % 2 == *odd){
            count++;
        }
    }
    printf("Count of %s numbers: %d\n", *odd ? "odd" : "even", count);
    int* returnValue = malloc(sizeof(int));
    *returnValue = count;
    return returnValue;
}


int main(int argc, char* argv[]){
    pthread_t t1, t2;
    int* w1 = malloc(sizeof(int));
    int* w0 = malloc(sizeof(int));
    w1[0] = 1;
    w0[0] = 0;
    for (int i= 0; i < arrLen; i++){
        arr[i] = i;
    }
    pthread_create(&t1, NULL, worker, w0);
    pthread_create(&t2, NULL, worker, w1);
    pthread_join(t1, (void**) w0);
    pthread_join(t2, (void**) w1);
    printf("%d\n%d\n", **(int**)w0, **(int**)w1);
    free(w1);
    free(w0);
}