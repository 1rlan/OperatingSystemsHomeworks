#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <semaphore.h>
#include <errno.h>
#include <signal.h>

#define SHM_NAME "/my_shm"
#define SEM_NAME "/my_sem"

volatile sig_atomic_t stop = 0;

void handle_signal(int signum) {
    stop = 1;
}

int main() {
    int fd_shm;
    sem_t *sem;

    // Установка обработчика сигнала
    signal(SIGINT, handle_signal);

    // Открытие сегмента разделяемой памяти
    if ((fd_shm = shm_open(SHM_NAME, O_RDWR, 0600)) == -1) {
        perror("Shm_open error");
        exit(EXIT_FAILURE);
    }

    // Маппинг сегмента разделяемой памяти в адресное пространство
    int *ptr_shm;
    if ((ptr_shm = mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED, fd_shm, 0)) == MAP_FAILED) {
        perror("Mmap error");
        exit(EXIT_FAILURE);
    }

    // Открытие семафора
    if ((sem = sem_open(SEM_NAME, 0)) == SEM_FAILED) {
        perror("Sem_open error");
        exit(EXIT_FAILURE);
    }

    // Генерация случайного числа и запись его в сегмент разделяемой памяти
    while (!stop) {
        int num = rand() % 100;
        *ptr_shm = num;
        sem_post(sem);
        sleep(1);
    }

    // Отсоединение от сегмента разделяемой памяти
    if (munmap(ptr_shm, sizeof(int)) == -1) {
        perror("Munmap error");
    }

    // Закрытие дескриптора сегмента разделяемой памяти
    if (close(fd_shm) == -1) {
        perror("Close error");
    }

    // Закрытие семафора
    if (sem_close(sem) == -1) {
        perror("Sem_close error");
    }

    return 0;
}
