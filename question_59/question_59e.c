/* Program Number: 59e
Student Name: Shruti Sijariya; Register Number: MT2026127
Date: 01-11-2026
Description: Write separate programs using the signal system call to catch the following signals:
              e. SIGALRM (using the setitimer system call)
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

#include <sys/time.h>

void handler(int signo) {
    printf("Caught SIGALRM (signal %d) from setitimer(ITIMER_REAL)\n", signo);
}

int main() {
    /*
     * CONCEPT: setitimer(ITIMER_REAL, ...)
     * ITIMER_REAL counts real (wall-clock) time and sends SIGALRM.
     * it_interval = 0 means the timer fires only once.
     */
    struct itimerval timer;
    timer.it_value.tv_sec = 3;
    timer.it_value.tv_usec = 0;
    timer.it_interval.tv_sec = 0;
    timer.it_interval.tv_usec = 0;

    signal(SIGALRM, handler);
    setitimer(ITIMER_REAL, &timer, NULL);

    printf("ITIMER_REAL set for 3 seconds, waiting...\n");
    pause();
    return 0;
}

/*
Output:
shrutis@ubuntu:~/SS_HO2/question_59$ gcc question_59e.c -o question_59e
shrutis@ubuntu:~/SS_HO2/question_59$ ./question_59e
ITIMER_REAL set for 3 seconds, waiting...
Caught SIGALRM (signal 14) from setitimer(ITIMER_REAL)
*/
