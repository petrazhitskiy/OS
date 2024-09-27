// o) pr1 | pr2 | ... | prn
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>


int main(int argc, char *argv[]) {
    if (argc < 2) {
        perror("No argv!");
        exit(1);
    }
    int num_commands = argc - 1;
    int pipe_fd[2];           // текущий канал
    printf("num_commands: %d\n", num_commands);
    int prev_fd = -1;         // для хранения предыдущего конца канала
    pid_t pid;

    for (int i = 0; i < num_commands; i++) {
        // Создаем канал для следующей команды, кроме последней
        if (i < num_commands - 1) {
            if (pipe(pipe_fd) == -1) {
                perror("pipe failed");
                exit(1);
            }
        }
        
        pid = fork();
        if (pid == -1) {
            perror("fork errkor");
            exit(1);
        }

        if (pid == 0) {
            if (i > 0) {
                // Если не первая команда, перенаправляем ВВОД НА ЧТЕНИЕ ДИСКРИПТОРА КАНАЛА 
                dup2(prev_fd, 0);
                close(prev_fd);
            }
            if (i < num_commands - 1) {
                // Если не последняя команда, перенаправляем ВЫВОД НА ЗАПИСЬ ДИСКРИПТОРОМ КАНАЛА 
                close(pipe_fd[0]);  
                dup2(pipe_fd[1], 1);
                close(pipe_fd[1]);
            }

            execlp(argv[i + 1], argv[i + 1], NULL);
            exit(1);
        }

        if (i > 0) {
            close(prev_fd); // Закрываем конец предыдущего канала
        }
        if (i < num_commands - 1) {
            close(pipe_fd[1]);
            // Запоминаем конец для чтения, чтобы передать следующему процессу
            prev_fd = pipe_fd[0];
        }
    }

    for (int i = 0; i < num_commands; i++) {
        wait(NULL);
    }

    return 0;
}
