#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <time.h>
#include <string.h>

#define SHM_KEY 1234
#define SHM_SIZE 1024

int main() {
    srand(time(NULL));

    int shm_id = shmget(SHM_KEY, SHM_SIZE, IPC_CREAT | 0666);
    if (shm_id == -1) {
        perror("Shmget error");
        exit(EXIT_FAILURE);
    }

    int *shm_ptr = (int *) shmat(shm_id, NULL, 0);
    if (shm_ptr == (void *) -1) {
        perror("Shmat error");
        exit(EXIT_FAILURE);
    }

    int ex = 0;
    printf("Для выхода из программы введите -1 и нажмите enter; иначе любое число :) \n");
    while (1) {
        int number = rand() % 1024;
        scanf("%d", &ex);
        if (ex == -1) {
            memcpy(shm_ptr, &ex, sizeof(number));
            break;
        } else {
            memcpy(shm_ptr, &number, sizeof(number));
        }
    }

    if (shmdt(shm_ptr) == -1) {
        perror("Shmdt error");
        exit(EXIT_FAILURE);
    }
}
