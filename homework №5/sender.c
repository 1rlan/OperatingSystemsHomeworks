#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

// Отправитель
int main() {
    char *myfifo = "/tmp/fifo";
    char buf[1024];

    int fd = open(myfifo, O_WRONLY);

    while (1) {
        printf("Write a message: ");
        fgets(buf, sizeof(buf), stdin);
        write(fd, buf, sizeof(buf));
    }
}
