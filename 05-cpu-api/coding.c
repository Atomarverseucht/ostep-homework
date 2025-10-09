#include <stdio.h>
#include <unistd.h>

void aufgabe1(){
    int x = 5;
    FILE *file = fopen("text.txt", "w");
    int pd[2];
    
    if(pipe(pd) < 0){
        printf("pipe failed");
    }
    int c = fork();
    
    if (c == 0) {
        // Child process
        printf("Hello from the child process!\n");
        fputs("Hi! ", file);
        write(pd, );
        x++;
    } else if(c > 0) {
        // Parent process

        printf("Hello from the parent process!\n");
        fputs("Bye! ", file);
        x--;
    } else {
        // Fork failed
        perror("Fork failed");
    }
    printf("%d\n", x);
    fclose(file);
}

aufgabe5(){
    int f = fork();
    if (f == 0) {
        // Child process
    } else if(f > 0) {
        // Parent process
    } else {
        // Fork failed
        perror("Fork failed");
    }
}

int main(char **argv, int argc) {
    aufgabe5();
    
    return 0;
}