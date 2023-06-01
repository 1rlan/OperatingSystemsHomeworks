#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <string.h>

#define SHM_KEY 1234
#define SHM_SIZE 1024

int main() {
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

    int prev = -1, number = -1;
    while (1) {
        memcpy(&number, shm_ptr, sizeof(number));
        if (number != prev) {
            printf("%d\n", number);
        }

        if (number == -1) {
            break;
        }

        prev = number;
    }

    if (shmdt(shm_ptr) == -1) {
        perror("Shmdt error");
        exit(EXIT_FAILURE);
    }

    if (shmctl(shm_id, IPC_RMID, NULL) == -1) {
        perror("Shmctl error");
        exit(EXIT_FAILURE);
    }
}
