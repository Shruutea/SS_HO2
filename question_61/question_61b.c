/* Program Number: 61b
Student Name: Shruti Sijariya; Register Number: MT2026127
Date: 01-11-2026
Description: Write separate programs using the sigaction system call to catch the following signals:
              b. SIGINT
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

void handler(int signo) {
    printf("\nsigaction caught SIGINT (signal %d): Ctrl+C pressed!\n", signo);
}

int main() {
    /*
     * CONCEPT: sigaction with SIGINT
     * Unlike old signal() on some systems, the handler set by sigaction
     * is NOT reset to default after it runs, so it stays installed.
     */
    struct sigaction sa;
    sa.sa_handler = handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sigaction(SIGINT, &sa, NULL);

    printf("PID %d: press Ctrl+C...\n", getpid());
    pause();

    printf("Handler finished, program exiting normally.\n");
    return 0;
}

/*
Output:
shrutis@ubuntu:~/SS_HO2/question_61$ gcc question_61b.c -o question_61b
shrutis@ubuntu:~/SS_HO2/question_61$ ./question_61b
PID 953: press Ctrl+C...
^C
sigaction caught SIGINT (signal 2): Ctrl+C pressed!
Handler finished, program exiting normally.
*/
