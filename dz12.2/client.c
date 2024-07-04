#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>

int main() {
    const char *fifo_name = "/tmp/my_fifo";

    int fd = open(fifo_name, O_RDONLY);

    if (fd == -1) {
        perror("open");
        exit(EXIT_FAILURE);
    }

    char message[1024];

    if (read(fd, message, sizeof(message)) == -1) {
        perror("read");
        exit(EXIT_FAILURE);
    }

    printf("Полученное сообщение: %s\n", message);

    close(fd);

    return 0;
}
