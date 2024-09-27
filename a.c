#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>
//a)  pr1 | pr2 | pr3
int main(int argc, char** argv) {
    int can1[2], can2[2];
    // Создаем pipe до fork
    pipe(can1);
    pipe(can2);
    // Первый процесс (pr1)
    if (fork() == 0) {
        // Перенаправляем stdout на can1[1]
        dup2(can1[1], 1);
        close(can1[0]); // Закрываем неиспользуемую часть
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
        dup2(can2[1], 1);
        close(can1[0]);
        close(can1[1]);
        close(can2[0]); // Закрываем неиспользуемую часть
        close(can2[1]);
        execlp(argv[2], argv[2], NULL); // pr2
        perror("execlp");
        exit(1);
    }

    // Закрываем неиспользуемые каналы в родительском процессе
    close(can1[0]);
    close(can1[1]);
    
    // Ждем завершения второго процесса
    wait(NULL);

    // Третий процесс (pr3)
    if (fork() == 0) {
        // Перенаправляем stdin на can2[0]
        dup2(can2[0], 0);
        close(can2[0]);
        close(can2[1]);

        execlp(argv[3], argv[3], NULL); // pr3
        perror("execlp");
        exit(1);
    }

    // Закрываем неиспользуемые каналы в родительском процессе
    close(can2[0]);
    close(can2[1]);

    // Ждем завершения третьего процесса
    wait(NULL);

    return 0;
}
