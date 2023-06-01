#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <semaphore.h>

#define MAX_MESSAGES 10
#define MAX_MESSAGE_SIZE 256

int main() {
    char message[MAX_MESSAGE_SIZE];

    int fd[2];
    if (pipe(fd) == -1) {
        perror("pipe");
        return 1;
    }

    sem_t *parent_sem, *child_sem;
    if ((parent_sem = sem_open("parent_sem", O_CREAT | O_EXCL, 0644, 1)) == SEM_FAILED) {
        perror("sem_open(parent_sem)");
        return 1;
    }

    if ((child_sem = sem_open("child_sem", O_CREAT | O_EXCL, 0644, 0)) == SEM_FAILED) {
        perror("sem_open(child_sem)");
        return 1;
    }

    pid_t pid = fork();
    if (pid < 0) {
        perror("fork");
        return 1;
    }

    if (pid == 0) {
        for (int i = 0; i < MAX_MESSAGES; ++i) {
            sem_wait(child_sem);
            read(fd[0], message, sizeof(message));
            printf("[child] received message: %s\n", message);
            sem_post(parent_sem);
        }
    } else {
        for (int i = 0; i < MAX_MESSAGES; ++i) {
            snprintf(message, sizeof(message), "Сообщение #%d", i + 1);
            sem_wait(parent_sem);

            write(fd[1], message, sizeof(message));
            printf("[parent] sent message: %s\n", message);
            sem_post(child_sem);
        }

        wait(NULL);
        close(fd[0]);
        close(fd[1]);
        sem_unlink("parent_sem");
        sem_unlink("child_sem");
    }

    return 0;
}
