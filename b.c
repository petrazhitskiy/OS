#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>

//b) pr1 | pr2 > f.res
int main(int argc, char** argv) {
    int can1[2];
    pipe(can1);
    int fl = open(argv[3], O_WRONLY | O_CREAT | O_TRUNC, 0776);
    if (fork() == 0) {
        dup2(can1[1], 1);
        close(can1[0]);
        close(can1[1]);
        execlp(argv[1], argv[1], NULL); // pr1
        perror("execlp");
        exit(1);
    }

    // Ждем завершения первого процесса
    wait(NULL);

    // Второй процесс (pr2)
    if (fork() == 0) {
        // Перенаправляем stdin на can1[0], stdout на can2[1]
        dup2(can1[0], 0);
        dup2(fl, 1);
        close(can1[0]);
        close(can1[1]);
        execlp(argv[2], argv[2], NULL); // pr2
        perror("execlp");
        exit(1);
    }

    // Закрываем неиспользуемые каналы в родительском процессе
    close(can1[0]);
    close(can1[1]);
    close(fl);
    
    // Ждем завершения второго процесса
    wait(NULL);
    // Ждем завершения третьего процесса
    wait(NULL);

    return 0;
}
