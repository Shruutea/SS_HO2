/* Program Number: 61a
Student Name: Shruti Sijariya; Register Number: MT2026127
Date: 01-11-2026
Description: Write separate programs using the sigaction system call to catch the following signals:
              a. SIGSEGV
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

void handler(int signo) {
    printf("sigaction caught SIGSEGV (signal %d): invalid memory access!\n", signo);
    exit(EXIT_FAILURE);
}

int main() {
    /*
     * CONCEPT: sigaction(signum, &new_action, &old_action)
     * A more reliable, portable version of signal().
     * sa_handler -> the handler function
     * sa_mask    -> extra signals blocked while the handler runs
     * sa_flags   -> options (0 = default behaviour)
     */
    struct sigaction sa;
    sa.sa_handler = handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sigaction(SIGSEGV, &sa, NULL);

    printf("Writing to a NULL pointer...\n");
    int *ptr = NULL;
    *ptr = 10;

    return 0;
}

/*
Output:
shrutis@ubuntu:~/SS_HO2/question_61$ gcc question_61a.c -o question_61a
shrutis@ubuntu:~/SS_HO2/question_61$ ./question_61a
Writing to a NULL pointer...
sigaction caught SIGSEGV (signal 11): invalid memory access!
*/
