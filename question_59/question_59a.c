/* Program Number: 59a
Student Name: Shruti Sijariya; Register Number: MT2026127
Date: 01-11-2026
Description: Write separate programs using the signal system call to catch the following signals:
              a. SIGSEGV
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

void handler(int signo) {
    printf("Caught SIGSEGV (signal %d): invalid memory access!\n", signo);
    exit(EXIT_FAILURE);   // must exit: returning would retry the bad access forever
}

int main() {
    /*
     * CONCEPT: signal(signum, handler)
     * Registers a function to run when the signal arrives.
     * SIGSEGV is sent when a process accesses memory it is not allowed to,
     * for example writing through a NULL pointer.
     */
    signal(SIGSEGV, handler);

    printf("Writing to a NULL pointer...\n");
    int *ptr = NULL;
    *ptr = 10;            // causes SIGSEGV

    printf("This line is never printed\n");
    return 0;
}

/*
Output:
shrutis@ubuntu:~/SS_HO2/question_59$ gcc question_59a.c -o question_59a
shrutis@ubuntu:~/SS_HO2/question_59$ ./question_59a
Writing to a NULL pointer...
Caught SIGSEGV (signal 11): invalid memory access!
*/
