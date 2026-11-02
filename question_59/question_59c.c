/* Program Number: 59c
Student Name: Shruti Sijariya; Register Number: MT2026127
Date: 01-11-2026
Description: Write separate programs using the signal system call to catch the following signals:
              c. SIGFPE
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

void handler(int signo) {
    printf("Caught SIGFPE (signal %d): arithmetic error (divide by zero)!\n", signo);
    exit(EXIT_FAILURE);   // must exit: returning would retry the division forever
}

int main() {
    /*
     * CONCEPT: SIGFPE
     * Sent on an arithmetic error such as INTEGER division by zero.
     * volatile stops the compiler from removing the division.
     */
    signal(SIGFPE, handler);

    volatile int a = 10, b = 0;
    printf("Dividing %d by %d...\n", a, b);
    int c = a / b;        // causes SIGFPE

    printf("Result: %d (never printed)\n", c);
    return 0;
}

/*
Output:
shrutis@ubuntu:~/SS_HO2/question_59$ gcc question_59c.c -o question_59c
shrutis@ubuntu:~/SS_HO2/question_59$ ./question_59c
Dividing 10 by 0...
Caught SIGFPE (signal 8): arithmetic error (divide by zero)!
*/
