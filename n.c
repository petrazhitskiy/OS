#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>

//n) pr1; pr2; ... ; prn &
int main(int argc, char** argv) {

    for (int i = 1; i < argc-1; ++i){
        if (fork() == 0) {
            execlp(argv[i], argv[i], NULL); // pr1
            perror("execlp");
            exit(1);
        }
        else 
            wait(NULL);
    }

    if (fork() == 0) {
        execlp(argv[argc-1], argv[argc-1], NULL); // pr1
        perror("execlp");
        exit(1);
    }
    return 0;
}




