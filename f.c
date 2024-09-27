#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>


//f)  pr1 | pr2 >> f.res
int main(int argc, char** argv) {
    if (argc < 4) {
        fprintf(stderr, "Usage: %s pr1 pr2 f.res\n", argv[0]);
        exit(1);
    }

    int can1[2];
    pipe(can1); // Создаем пайп для связи pr1 -> pr2
    int fl = open(argv[3], O_WRONLY | O_CREAT | O_APPEND, 0776); // Открываем файл для добавления (>>)

    if (fl == -1) {
        perror("Failed to open file");
        exit(1);
    }

    // Первый процесс (pr1)
    if (fork() == 0) {
        dup2(can1[1], 1);   // Перенаправляем stdout pr1 в пайп
        close(can1[0]);
        close(can1[1]);
        execlp(argv[1], argv[1], NULL); // Выполняем pr1
        perror("execlp pr1 failed");
        exit(1);
    }

    // Ждем завершения первого процесса
    wait(NULL);

    // Второй процесс (pr2)
    if (fork() == 0) {
        dup2(can1[0], 0);   // Перенаправляем stdin pr2 на чтение из пайпа
        dup2(fl, 1);        // Перенаправляем stdout pr2 на файл с добавлением
        close(can1[0]);
        close(can1[1]);
        execlp(argv[2], argv[2], NULL); // Выполняем pr2
        perror("execlp pr2 failed");
        exit(1);
    }

    // Закрываем каналы и файл в родительском процессе
    close(can1[0]);
    close(can1[1]);
    close(fl);
    
    // Ждем завершения второго процесса
    wait(NULL);

    return 0;
}
