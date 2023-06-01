#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 12345
#define MAX_BUFFER_SIZE 1024

int main() {
    int sockfd;
    struct sockaddr_in serverAddr;
    char buffer[MAX_BUFFER_SIZE];

    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
        perror("Ошибка создания сокета");
        return 1;
    }

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    memset(serverAddr.sin_zero, '\0', sizeof(serverAddr.sin_zero));

    if (bind(sockfd, (struct sockaddr *) &serverAddr, sizeof(serverAddr)) == -1) {
        perror("Ошибка привязки сокета");
        return 1;
    }

    while (1) {
        struct sockaddr_in senderAddr;
        socklen_t senderAddrLen = sizeof(senderAddr);
        ssize_t bytesRead = recvfrom(sockfd, buffer, sizeof(buffer), 0, (struct sockaddr *) &senderAddr, &senderAddrLen);
        if (bytesRead == -1) {
            perror("Ошибка приема сообщения");
            return 1;
        }

        buffer[bytesRead] = '\0';
        printf("Получено сообщение от сервера: %s\n", buffer);
    }

    close(sockfd);
    return 0;
}