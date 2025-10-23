#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/sysinfo.h>

int main(int argc, char **argv) {
    if (argc != 2) {
        fprintf(stderr, "%s <amount in MiB>\n", argv[0]);
        return 1;
    }

    errno = 0;
    unsigned long long mib = strtoull(argv[1], NULL, 10);

    size_t bytes = (size_t)mib * 1024 * 1024; /* MiB -> bytes */
    char *arr = malloc(bytes);

    /* Touch one byte per page to fault pages into RAM */
    size_t page = (size_t) getpagesize();
    volatile char *v = (volatile char *)arr;
    for (size_t i = 0; i < bytes; i += page) {
        v[i] = 1;
    }

    printf("Allocated and touched %llu MiB (= %zu bytes). PID=%d\n", mib, bytes, getpid());
    printf("Sleeping 60s so you can inspect memory (top/pmap/ps /proc/<pid>/status)...\n");
    //sleep(60);

    /* optionally keep running so you can watch */
    sleep(100000);

    free(arr);
    return 0;
}