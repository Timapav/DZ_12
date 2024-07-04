#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h> 

int main() {
    int pipe_fds[2]; 

    if (pipe(pipe_fds) < 0) {
        perror("pipe");
        exit(1);
    }

    int pid = fork(); 
    if (pid < 0) {
        perror("fork");
        exit(1);
    }

    if (pid == 0) {
    
        close(pipe_fds[1]); 

        char message[1024];
        if (read(pipe_fds[0], message, sizeof(message)) == -1) {
            perror("read");
            exit(1);
        }

        printf("Полученное сообщение: %s\n", message);

        close(pipe_fds[0]); 
    } else {
      
        close(pipe_fds[0]);  

        char message[] = "Hi!"; 
        if (write(pipe_fds[1], message, strlen(message) + 1) == -1) {
            perror("write");
            exit(1);
        }

        wait(NULL);  

        close(pipe_fds[1]); 
    }

    return 0;
}
