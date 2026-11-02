/* Program Number: 59g
Student Name: Shruti Sijariya; Register Number: MT2026127
Date: 01-11-2026
Description: Write separate programs using the signal system call to catch the following signals:
              g. SIGPROF (using the setitimer system call)
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

#include <sys/time.h>

volatile int caught = 0;

void handler(int signo) {
    printf("Caught SIGPROF (signal %d) from setitimer(ITIMER_PROF)\n", signo);
    caught = 1;
}

int main() {
    /*
     * CONCEPT: setitimer(ITIMER_PROF, ...)
     * Counts user + kernel CPU time of the process and sends SIGPROF.
     */
    struct itimerval timer;
    timer.it_value.tv_sec = 3;
    timer.it_value.tv_usec = 0;
    timer.it_interval.tv_sec = 0;
    timer.it_interval.tv_usec = 0;

    signal(SIGPROF, handler);
    setitimer(ITIMER_PROF, &timer, NULL);

    printf("ITIMER_PROF set for 3 seconds of CPU time, busy looping...\n");
    while (!caught) {
        // busy loop
    }
    return 0;
}

/*
Output:
shrutis@ubuntu:~/SS_HO2/question_59$ gcc question_59g.c -o question_59g
shrutis@ubuntu:~/SS_HO2/question_59$ ./question_59g
ITIMER_PROF set for 3 seconds of CPU time, busy looping...
Caught SIGPROF (signal 27) from setitimer(ITIMER_PROF)
*/
