#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#define PORT 5555

int main(int argc, char const *argv[]) {
    int sock = 0;
    struct sockaddr_in serv_addr;
    char buffer[1024] = {0};

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("Ошибка при создании сокета\n");
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    if (inet_pton(AF_INET, argv[1], &serv_addr.sin_addr) <= 0) {
        printf("Указан неправильный IP\n");
        return -1;
    }

    if (connect(sock, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
        printf("Не удалось подключиться к серверу\n");
        return -1;
    }

    while (1) {
        memset(buffer, 0, sizeof(buffer));
        int valread = read(sock, buffer, 1024);

        if (strcmp(buffer, "The End\n") == 0) {
            printf("Сервер закрылся, расходимся\n");
            break;
        }

        printf("Сообщение от другого клиента: %s", buffer);
    }

    close(sock);
    return 0;
}
