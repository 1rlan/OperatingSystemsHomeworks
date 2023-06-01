#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

// Получатель
int main() {
    char *fifo = "/tmp/fifo";
    char buf[1024];

    mkfifo(fifo, 0666);
    int fd = open(fifo, O_RDONLY);

    while (1) {
        if (read(fd, buf, sizeof(buf)) > 0) {
            printf("New message received:\n%s", buf);
        }
    }
}
