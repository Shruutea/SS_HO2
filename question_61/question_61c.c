/* Program Number: 61c
Student Name: Shruti Sijariya; Register Number: MT2026127
Date: 01-11-2026
Description: Write separate programs using the sigaction system call to catch the following signals:
              c. SIGFPE
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

void handler(int signo) {
    printf("sigaction caught SIGFPE (signal %d): divide by zero!\n", signo);
    exit(EXIT_FAILURE);
}

int main() {
    /*
     * CONCEPT: sigaction with SIGFPE (integer division by zero)
     */
    struct sigaction sa;
    sa.sa_handler = handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sigaction(SIGFPE, &sa, NULL);

    volatile int a = 10, b = 0;
    printf("Dividing %d by %d...\n", a, b);
    int c = a / b;

    printf("Result: %d (never printed)\n", c);
    return 0;
}

/*
Output:
shrutis@ubuntu:~/SS_HO2/question_61$ gcc question_61c.c -o question_61c
shrutis@ubuntu:~/SS_HO2/question_61$ ./question_61c
Dividing 10 by 0...
sigaction caught SIGFPE (signal 8): divide by zero!
*/
