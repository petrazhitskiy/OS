#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>

//c)  pr1 arg11 arg12 < f.dat | pr2 arg21 agr22
int main(int argc, char** argv) {
    int can1[2];
    pipe(can1);
    int fl = open(argv[4], O_RDONLY | O_CREAT, 0776);
    if (fork() == 0) {
        dup2(can1[1], 1);
        dup2(fl, 0);
        close(can1[0]);
        close(can1[1]);
        close(fl);
        execlp(argv[1], argv[1], argv[2], argv[3], NULL); // pr1
        perror("execlp");
        exit(1);
    }

    // Ждем завершения первого процесса
    wait(NULL);

    if (fork() == 0) {
        dup2(can1[0], 0);
        close(can1[0]);
        close(can1[1]);
        close(fl);
        execlp(argv[5], argv[5],argv[6],argv[7], NULL); // pr2
        perror("execlp");
        exit(1);
    }

    // Закрываем неиспользуемые каналы в родительском процессе
    close(can1[0]);
    close(can1[1]);
    close(fl);
    
    // Ждем завершения третьего процесса
    wait(NULL);

    return 0;
}
