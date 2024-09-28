#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/wait.h>

int main(int argc, char** argv) {
    if (argc < 6) {
        fprintf(stderr, "Usage: %s pr1 arg1 pr2 pr3 f.dat f.res\n", argv[0]);
        exit(1);
    }

    int can1[2], can2[2];
    pipe(can1); // Создаем первый канал для связи pr1 -> pr2
    pipe(can2); // Создаем второй канал для связи pr2 -> pr3

    // Открываем файл f.dat для чтения
    int fdat = open(argv[5], O_RDONLY);
    if (fdat == -1) {
        perror("Failed to open f.dat");
        exit(1);
    }

    // Открываем файл f.res для записи в режиме добавления (>>)
    int fres = open(argv[6], O_WRONLY | O_CREAT | O_APPEND, 0776);
    if (fres == -1) {
        perror("Failed to open f.res");
        close(fdat);
        exit(1);
    }

    // Процесс 1: pr1 arg1 < f.dat
    if (fork() == 0) {
        dup2(fdat, 0);
        dup2(can1[1], 1);
        
        close(fdat);
        close(fres);
        close(can1[0]);
        close(can1[1]);
        close(can2[0]);
        close(can2[1]);
        
        execlp(argv[1], argv[1], argv[2], NULL);
        perror("execlp pr1 failed");
        exit(1);
    }

    close(fdat);
    wait(NULL);

    // Процесс 2: pr2
    if (fork() == 0) {
        dup2(can1[0], 0);
        dup2(can2[1], 1);
        close(fres);
        close(can1[0]);
        close(can1[1]);
        close(can2[0]);
        close(can2[1]);
        
        execlp(argv[3], argv[3], NULL);
        perror("execlp pr2 failed");
        exit(1);
    }

    // Закрываем первый канал, так как он больше не нужен в родительском процессе
    close(can1[0]);
    close(can1[1]);

    // Ждем завершения pr2
    wait(NULL);

    // Процесс 3: pr3 >> f.res
    if (fork() == 0) {
        dup2(can2[0], 0);
        dup2(fres, 1);

        close(fres);
        close(can2[0]);
        close(can2[1]);

        execlp(argv[4], argv[4], NULL);
        perror("execlp pr3 failed");
        exit(1);
    }
    // Закрываем второй канал и файл f.res в родительском процессе
    close(can2[0]);
    close(can2[1]);
    close(fres);
    wait(NULL);

    return 0;
}
