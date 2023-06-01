#include <signal.h>
#include <stdio.h>
#include <unistd.h>

int receiver_pid;

void wait() {}

void send(int bit) {
    if (bit == 0) {
        kill(receiver_pid, SIGUSR1);
    } else {
        kill(receiver_pid, SIGUSR2);
    }
}

int main(void) {
    signal(SIGUSR1, wait);

    int sender_pid = getpid();
    printf("Client PID = %d\n", sender_pid);

    printf("Enter server PID: ");
    scanf("%d", &receiver_pid);

    int num;
    printf("Number to send: ");
    scanf("%d", &num);

    for (int i = 0; i < 32; ++i) {
        send(num & 1);
        num >>= 1;
        pause();
    }

    return 0;
}