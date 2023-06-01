#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>Z

#define PORT 12345
#define MAX_BUFFER_SIZE 1024

int main() {
    int sockfd;
    struct sockaddr_in serverAddr, clientAddr;
    char buffer[MAX_BUFFER_SIZE];

    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
        perror("Ошибка создания сокета");
        return 1;
    }

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    memset(serverAddr.sin_zero, '\0', sizeof(serverAddr.sin_zero));

    if (bind(sockfd, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == -1) {
        perror("Ошибка привязки сокета");
        return 1;
    }

    while (1) {
        printf("Введите сообщение для отправки: ");
        fgets(buffer, sizeof(buffer), stdin);

        struct sockaddr_in broadcastAddr;
        memset(&broadcastAddr, 0, sizeof(broadcastAddr));
        broadcastAddr.sin_family = AF_INET;
        broadcastAddr.sin_port = htons(PORT);
        broadcastAddr.sin_addr.s_addr = htonl(INADDR_BROADCAST);

        if (sendto(sockfd, buffer, strlen(buffer), 0, (struct sockaddr*)&broadcastAddr, sizeof(broadcastAddr)) == -1) {
            perror("Ошибка отправки сообщения");
            return 1;
        }
    }

    close(sockfd);
    return 0;
}
