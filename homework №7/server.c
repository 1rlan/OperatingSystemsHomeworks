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

    // Создание сегмента разделяемой памяти
    if ((fd_shm = shm_open(SHM_NAME, O_RDWR | O_CREAT | O_TRUNC, 0600)) == -1) {
        perror("Shm_open error");
        exit(EXIT_FAILURE);
    }
    if (ftruncate(fd_shm, sizeof(int)) == -1) {
        perror("Ftruncate error");
        exit(EXIT_FAILURE);
    }

    // Маппинг сегмента разделяемой памяти в адресное пространство
    int *ptr_shm;
    if ((ptr_shm = mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED, fd_shm, 0)) == MAP_FAILED) {
        perror("Mmap error");
        exit(EXIT_FAILURE);
    }

    // Инициализация семафора
    if ((sem = sem_open(SEM_NAME, O_CREAT, 0600, 0)) == SEM_FAILED) {
        perror("Sem_open error");
        exit(EXIT_FAILURE);
    }

    // Ожидание подключения клиента и вывод числа на экран
    while (!stop) {
        sem_wait(sem);
        printf("Received number: %d\n", *ptr_shm);
    }

    // Отсоединение от сегмента разделяемой памяти
    if (munmap(ptr_shm, sizeof(int)) == -1) {
        perror("Munmap error");
    }

    // Закрытие дескриптора сегмента разделяемой памяти
    if (close(fd_shm) == -1) {
        perror("Close error");
    }

    // Удаление сегмента разделяемой памяти
    if (shm_unlink(SHM_NAME) == -1) {
        perror("Shm_unlink error");
    }

    // Закрытие семафора
    if (sem_close(sem) == -1) {
        perror("Sem_close error");
    }

    // Удаление семафора
    if (sem_unlink(SEM_NAME) == -1) {
        perror("Sem_unlink error");
    }

    return 0;
}
