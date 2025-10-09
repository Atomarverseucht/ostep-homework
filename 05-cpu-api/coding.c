#define _GNU_SOURCE
#include <stdio.h>
#include <unistd.h>

extern char** environ;

void aufgabe1bis3(){
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
        write(pd[1], "h",2);
        x++;
    } else if(c > 0) {
        // Parent process
        char c[2];
        read(pd[0], c, 2);
        printf("Hello from the parent process!%s\n", c);
        fputs("Bye! ", file);
        x--;
    } else {
        // Fork failed
        perror("Fork failed");
    }
    printf("%d\n", x);
    fclose(file);
}

void aufgabe4(){
    int f = fork();
    if (f == 0) {
        // Child process
        char* arg[3];
        arg[0] = "ls";
        arg[1] = "-la";
        arg[2] = NULL;
        execvpe("ls", arg, environ);
    } else if(f > 0) {
        // Parent process
        printf("Hi ");
    } else {
        // Fork failed
        perror("Fork failed");
    }
}

int main(char **argv, int argc) {
    aufgabe4();
    
    return 0;
}