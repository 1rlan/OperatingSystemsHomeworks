#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int fibonacci(int n) {
    if (n == 0 || n == 1) {
        return n;
    } else {
        return fibonacci(n - 1) + fibonacci(n - 2);
    }
}

int factorial(int n) {
    if (n == 0 || n == 1) {
        return 1;
    } else {
        return n * factorial(n - 1);
    }
}

int main() {
    int n;
    scanf("%d", &n);
    pid_t pid = fork();

    if (pid == 0) {
        printf("Факториал %d = %d\n", n, factorial(n));
        exit(0);
    } else if (pid > 0) {
        printf("Число фибоначи %d = %d\n", n, fibonacci(n));
        wait(NULL);
    } else {
        return 1;
    }

    return 0;
}
