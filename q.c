#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc, char *argv[]) {
    if (argc < 3){
        perror("argv error\n");
    }
    
    int st;
    pid_t pid; 
    if ((pid = fork()) == 0) {  
        execlp(argv[1], argv[1], NULL); 
        exit(1); 
    } else if (pid > 0) { 
        wait(&st);
        if (st != 0) {
            if (fork() == 0) {
                execlp(argv[2], argv[2], NULL);
                exit(1); 
            }
            wait(NULL);
        }
    } else {
        perror("fork error");
        exit(1);
    }
    
    return 0;
}
