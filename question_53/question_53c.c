/* Program Number: 53c
Student Name: Shruti Sijariya; Register Number: MT2026127
Date: 01-11-2026
Description: Write separate programs for each time domain to set an interval timer
              for 10 seconds and 10 microseconds:
              c. Use ITIMER_PROF
*/

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <time.h>
#include <sys/time.h>

volatile int count = 0;
time_t start;

void handler(int signo) {
    count++;
    printf("[t=%lds] SIGPROF received (expiry %d)\n", (long)(time(NULL) - start), count);
}

int main() {
    /*
     * CONCEPT: ITIMER_PROF
     * Counts CPU time in BOTH user mode and kernel mode
     * (user time + system time). Used by profilers.
     * On expiry the kernel sends SIGPROF.
     */
    struct itimerval timer;
    timer.it_value.tv_sec = 10;
    timer.it_value.tv_usec = 10;
    timer.it_interval.tv_sec = 10;
    timer.it_interval.tv_usec = 10;

    signal(SIGPROF, handler);

    start = time(NULL);
    if (setitimer(ITIMER_PROF, &timer, NULL) == -1) {
        perror("setitimer failed");
        exit(EXIT_FAILURE);
    }
    printf("[t=%lds] ITIMER_PROF set for 10 s 10 us of CPU time\n", (long)(time(NULL) - start));

    while (count < 1) {
        // busy loop: uses CPU time
    }
    return 0;
}

/*
Output:
shrutis@ubuntu:~/SS_HO2/question_53$ gcc question_53c.c -o question_53c
shrutis@ubuntu:~/SS_HO2/question_53$ ./question_53c
[t=0s] ITIMER_PROF set for 10 s 10 us of CPU time
[t=10s] SIGPROF received (expiry 1)
*/
