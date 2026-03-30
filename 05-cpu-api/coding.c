#define _GNU_SOURCE
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>

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
        execvpe(arg[0], arg, environ);
    } else if(f > 0) {
        // Parent process
        printf("Hi ");
    } else {
        // Fork failed
        perror("Fork failed");
    }
}
void aufgabe5(){
    int rc = fork();
    if(rc < 0){
        printf("fork failed");
        exit(1);
    } else if(rc == 0){
        printf("child\n");
    } else {
        wait(NULL);
        printf("parent\n");
    }
}
void aufgabe6(){
    int rc = fork();
    if(rc < 0){
        printf("fork failed");
        exit(1);
    } else if(rc == 0){
        printf("child\n");
    } else {
        waitpid(rc, NULL, NULL);
        printf("parent\n");
    }
}

void aufgabe7(){
    int rc = fork();
    if(rc < 0){
        printf("fork failed");
        exit(1);
    } else if(rc == 0){
        close(STDOUT_FILENO);
        printf("child\n");
    } else {
        printf("parent\n");
    }
}

void aufgabe8(){
    int pipefd[2];
    pipe(pipefd);
    int rc = fork();
    if(rc < 0){
        printf("fork failed");
        exit(1);
    } else if(rc == 0){
        dup2(pipefd[1], STDOUT_FILENO);
        printf("child ");
    } else {
        dup2(pipefd[0], STDIN_FILENO);
        char buf[1024];
        ssize_t n = read(STDIN_FILENO, buf, sizeof(buf));
        printf("%.*sfhgfmhgd ", (int)n, buf);
    }
}

int main(char **argv, int argc) {
    aufgabe8();
    
    return 0;
}