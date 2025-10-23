#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/sysinfo.h>

int main(int argc, char **argv) {
    int* pointer = NULL;
    int num = 5;
    pointer = &num;
    
    num = *pointer;
}
