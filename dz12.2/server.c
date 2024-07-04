#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>

int main() {
    const char *fifo_name = "/tmp/my_fifo";

    if (mkfifo(fifo_name, 0666) == -1) {
        perror("mkfifo");
        exit(EXIT_FAILURE);
    }

    int fd = open(fifo_name, O_WRONLY);

    if (fd == -1) {
        perror("open");
        exit(EXIT_FAILURE);
    }

    const char *message = "Hi!";

    if (write(fd, message, strlen(message) + 1) == -1) {
        perror("write");
        exit(EXIT_FAILURE);
    }

    close(fd);

    return 0;
}
