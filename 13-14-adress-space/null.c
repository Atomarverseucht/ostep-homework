#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/sysinfo.h>

int main(int argc, char **argv) {
    int* pointer = malloc(sizeof(int));
    free(pointer);
    int* data = malloc(sizeof(int)*10);
    data[3] = 15;
    data = realloc(data, sizeof(int)*100);
    data[91] = 10;
    printf("%d\n", data[3]);
    free(data);
    /*int num = 5;
    pointer = &num;
    
    num = *pointer;*/
}
