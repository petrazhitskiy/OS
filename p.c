#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

//p) pr1 | pr2 | ... | prn &

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s cmd1 cmd2 ... cmdn\n", argv[0]);
        exit(1);
    }

    int num_commands = argc - 1; // Количество команд
    int pipe_fd[2];              // Текущий пайп
    int prev_fd = -1;            // Для хранения предыдущего конца пайпа
    pid_t pid;

    for (int i = 0; i < num_commands; i++) {
        if (i < num_commands - 1) {
            // Создаем пайп, если это не последняя команда
            if (pipe(pipe_fd) == -1) {
                perror("pipe failed");
                exit(1);
            }
        }

        pid = fork();
        if (pid == -1) {
            perror("fork failed");
            exit(1);
        }

        if (pid == 0) {  // Дочерний процесс
            if (i > 0) {
                // Если это не первая команда, перенаправляем ввод
                dup2(prev_fd, 0);
                close(prev_fd);
            }

            if (i < num_commands - 1) {
                // Если это не последняя команда, перенаправляем вывод
                close(pipe_fd[0]);  // Закрываем чтение
                dup2(pipe_fd[1], 1); // Перенаправляем вывод на запись в пайп
                close(pipe_fd[1]);   // Закрываем запись после перенаправления
            }

            execlp(argv[i + 1], argv[i + 1], NULL); // Выполняем команду
            perror("execlp failed");
            exit(1);
        }

        // Родительский процесс
        if (i > 0) {
            close(prev_fd);  // Закрываем предыдущий конец пайпа
        }

        if (i < num_commands - 1) {
            close(pipe_fd[1]);  // Закрываем конец для записи в текущий пайп
            prev_fd = pipe_fd[0]; // Запоминаем конец для чтения следующей команды
        }
    }

    // В отличие от обычного режима, мы не ждем завершения дочерних процессов (запуск в фоне)
    return 0;  // Родительский процесс завершается немедленно
}
