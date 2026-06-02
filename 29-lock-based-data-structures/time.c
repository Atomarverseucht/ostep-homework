#include <stdio.h>
#include <sys/time.h>

int main() {
    struct timeval start, end;
    gettimeofday(&start, NULL);

    // Eine kleine Verzögerung/Arbeit simulieren
    for (volatile int i = 0; i < 1000000; i++);

    gettimeofday(&end, NULL);

    long seconds = end.tv_sec - start.tv_sec;
    long microseconds = end.tv_usec - start.tv_usec;
    double elapsed = seconds + microseconds*1e-6;

    printf("Vergangene Zeit: %.6f Sekunden\n", elapsed);
    return 0;
}