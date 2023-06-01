#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#define PORT 5555

int main(int argc, char const *argv[]) {
    int server_fd, client_sockets[2], max_sd, activity, valread;
    struct sockaddr_in address;
    char buffer[1024];
    fd_set readfds;

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // привязка сокета к адресу и порту сервера
    if (bind(server_fd, (struct sockaddr *) &address, sizeof(address)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    // прослушивание входящих соединений
    if (listen(server_fd, 2) < 0) {
        perror("listen failed");
        exit(EXIT_FAILURE);
    }

    printf("Ждем подключения клиентов...\n");
    for (int i = 0; i < 2; i++) {
        if ((client_sockets[i] = accept(server_fd, (struct sockaddr *) &address, (socklen_t *) &address)) < 0) {
            perror("accept failed");
            exit(EXIT_FAILURE);
        }
        printf("Клиент %d подключен\n", i + 1);
    }

    while (1) {
        FD_ZERO(&readfds);

        for (int i = 0; i < 2; i++) {
            FD_SET(client_sockets[i], &readfds);
        }

        max_sd = client_sockets[0];
        for (int i = 1; i < 2; i++) {
            if (client_sockets[i] > max_sd) {
                max_sd = client_sockets[i];
            }
        }

        activity = select(max_sd + 1, &readfds, NULL, NULL, NULL);
        if (activity < 0) {
            perror("select failed");
            exit(EXIT_FAILURE);
        }

        for (int i = 0; i < 2; i++) {
            if (FD_ISSET(client_sockets[i], &readfds)) {
                valread = read(client_sockets[i], buffer, 1024);
                if (valread == 0) {
                    printf("Клиент %d отсоединился\n", i + 1);
                    close(client_sockets[i]);
                    client_sockets[i] = 0;
                } else {
                    int dest;
                    if (i == 0) {
                        dest = 1;
                    } else {
                        dest = 0;
                    }

                    if (client_sockets[dest] != 0) {
                        write(client_sockets[dest], buffer, strlen(buffer));
                    }

                    printf("Получено сообщение от клиента %d: %s", i + 1, buffer);
                }
            }
        }

        if (client_sockets[0] == client_sockets[1] == 0) {
            printf("All clients disconnected. Exiting...\n");
            break;
        }
    }

    for (int i = 0; i < 2; i++) {
        if (client_sockets[i] != 0) {
            close(client_sockets[i]);
        }
    }
    
    close(server_fd);
    return 0;
}