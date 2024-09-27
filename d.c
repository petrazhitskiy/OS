#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>

//d) pr1 < f.dat > f.res
int main(int argc, char** argv) {
    int infl = open(argv[2], O_RDONLY | O_CREAT, 0776);
    int outfl = open(argv[3], O_WRONLY | O_CREAT | O_TRUNC, 0776);
    if (fork() == 0) {
        dup2(infl, 0);
        dup2(outfl, 1);
        close(infl);
        close(outfl);
        execlp(argv[1], argv[1], NULL); // pr1
        perror("execlp");
        exit(1);
    }

    // Ждем завершения первого процесса
    wait(NULL);
    close(infl);
    close(outfl);

    return 0;
}
