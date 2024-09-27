#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>

//m) pr1; pr2; ... ; prn
int main(int argc, char** argv) {

    for (int i = 1; i < argc; ++i){
        if (fork() == 0) {
            execlp(argv[i], argv[i], NULL); // pr1
            perror("execlp");
            exit(1);
        }
        else 
            wait(NULL);
    }

    return 0;
}
