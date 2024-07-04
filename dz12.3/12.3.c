#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

#define MAX_INPUT 100
#define MAX_ARGS 10

void parse_command(char *command, char **args) {
    int i = 0;
    args[i] = strtok(command, " ");
    while (args[i] != NULL && i < MAX_ARGS - 1) {
        args[++i] = strtok(NULL, " ");
    }
    args[i] = NULL;
}

int main() {
    char command[MAX_INPUT];
    char *args[MAX_ARGS];
    char *args_pipe[MAX_ARGS];
    pid_t pid;
    int fd[2];

    while (1) {
        printf("Введите команду: ");
        if (fgets(command, sizeof(command), stdin) == NULL) {
            perror("Ошибка чтения команды");
            continue;
        }
        command[strcspn(command, "\n")] = 0;

        if (strcmp(command, "exit") == 0) {
            break;
        }

        char *pipe_pos = strchr(command, '|');
        if (pipe_pos != NULL) { 
            *pipe_pos = 0;
            pipe_pos++;
            while (*pipe_pos == ' ') pipe_pos++; 

            parse_command(command, args);
            parse_command(pipe_pos, args_pipe);

            if (pipe(fd) == -1) {
                perror("Ошибка создания канала");
                continue;
            }

            if ((pid = fork()) == 0) {
                close(fd[0]); 
                dup2(fd[1], STDOUT_FILENO); 
                close(fd[1]);
                execvp(args[0], args);
                perror("Ошибка выполнения команды");
                exit(EXIT_FAILURE);
            } else if (pid < 0) {
                perror("Ошибка создания процесса");
                continue;
            }

            if ((pid = fork()) == 0) {
                close(fd[1]); 
                dup2(fd[0], STDIN_FILENO); 
                close(fd[0]);
                execvp(args_pipe[0], args_pipe);
                perror("Ошибка выполнения команды");
                exit(EXIT_FAILURE);
            } else if (pid < 0) {
                perror("Ошибка создания процесса");
                continue;
            }

            close(fd[0]);
            close(fd[1]);

            wait(NULL);
            wait(NULL);
        } else {
            parse_command(command, args);

            pid = fork();
            if (pid == 0) {
                execvp(args[0], args);
                perror("Ошибка выполнения команды");
                exit(EXIT_FAILURE);
            } else if (pid > 0) {
                wait(NULL);
            } else {
                perror("Ошибка создания процесса");
            }
        }
    }

    return 0;
}
