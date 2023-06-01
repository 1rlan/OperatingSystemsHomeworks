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

    // создание сокета
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("Socket creation error\n");
        return -1;
    }

    // настройка адреса сервера
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    // конвертация IP-адреса из текстового вида в двоичный
    if (inet_pton(AF_INET, argv[1], &serv_addr.sin_addr) <= 0) {
        printf("Invalid address/ Address not supported\n");
        return -1;
    }

    // подключение к серверу
    if (connect(sock, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
        printf("Connection failed\n");
        return -1;
    }

    // чтение сообщений с клавиатуры и отправка на сервер
    while (1) {
        printf("Введите сообщение: ");
        fgets(buffer, 1024, stdin);

        write(sock, buffer, strlen(buffer));

        // проверка на завершение работы
        if (strcmp(buffer, "The End\n") == 0) {
            break;
        }
    }

    // закрытие сокета
    close(sock);

    return 0;
}
