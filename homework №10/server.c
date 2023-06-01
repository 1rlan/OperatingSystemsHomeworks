#include <signal.h>
#include <stdio.h>
#include <unistd.h>

int sender_pid;
int received_bit = -1;

void handler(int nsig) {
    if (nsig == SIGUSR1) {
        received_bit = 0;
    } else if (nsig == SIGUSR2) {
        received_bit = 1;
    }
}

int main(void) {
    (void) signal(SIGUSR1, handler);
    (void) signal(SIGUSR2, handler);

    int receiver_pid = getpid();
    printf("Server PID = %d\n", receiver_pid);

    printf("Enter client PID: ");
    scanf("%d", &sender_pid);

    kill(sender_pid, SIGUSR1);

    int bits[32];
    for (int i = 0; i < 32; ++i) {
        pause();

        bits[i] = received_bit;
        printf("%d", received_bit);
        kill(sender_pid, SIGUSR1);
    }

    int num = 0;
    for (int i = 0; i < 32; ++i) {
        num |= bits[i] << i;
    }

    printf("\n[server] number %d received\n", num);
    return 0;
}