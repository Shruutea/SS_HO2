/* Program Number: 53b
Student Name: Shruti Sijariya; Register Number: MT2026127
Date: 01-11-2026
Description: Write separate programs for each time domain to set an interval timer
              for 10 seconds and 10 microseconds:
              b. Use ITIMER_VIRTUAL
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
    printf("[t=%lds] SIGVTALRM received (expiry %d)\n", (long)(time(NULL) - start), count);
}

int main() {
    /*
     * CONCEPT: ITIMER_VIRTUAL
     * Counts only the CPU time the process spends in USER mode.
     * Time spent sleeping or in the kernel is NOT counted,
     * so the process must keep the CPU busy (the empty loop).
     * On expiry the kernel sends SIGVTALRM.
     */
    struct itimerval timer;
    timer.it_value.tv_sec = 10;
    timer.it_value.tv_usec = 10;
    timer.it_interval.tv_sec = 10;
    timer.it_interval.tv_usec = 10;

    signal(SIGVTALRM, handler);

    start = time(NULL);
    if (setitimer(ITIMER_VIRTUAL, &timer, NULL) == -1) {
        perror("setitimer failed");
        exit(EXIT_FAILURE);
    }
    printf("[t=%lds] ITIMER_VIRTUAL set for 10 s 10 us of user CPU time\n", (long)(time(NULL) - start));

    while (count < 1) {
        // busy loop: uses user-mode CPU time
    }
    return 0;
}

/*
Output:
shrutis@ubuntu:~/SS_HO2/question_53$ gcc question_53b.c -o question_53b
shrutis@ubuntu:~/SS_HO2/question_53$ ./question_53b
[t=0s] ITIMER_VIRTUAL set for 10 s 10 us of user CPU time
[t=10s] SIGVTALRM received (expiry 1)
*/
