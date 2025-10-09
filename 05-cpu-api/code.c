#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

int main(int argc, char* argv[]){
    int f = open("./text.txt",O_WRONLY,NULL);
    dup2(f, STDOUT_FILENO);
    printf("hibahguqhedgquog");
}