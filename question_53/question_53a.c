/* Program Number: 53a
Student Name: Shruti Sijariya; Register Number: MT2026127
Date: 01-11-2026
Description: Write separate programs for each time domain to set an interval timer
              for 10 seconds and 10 microseconds:
              a. Use ITIMER_REAL
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>
#include <sys/time.h>

int count = 0;
time_t start;

void handler(int signo) {
    count++;
    printf("[t=%lds] SIGALRM received (expiry %d)\n", (long)(time(NULL) - start), count);
}

int main() {
    /*
     * CONCEPT: ITIMER_REAL
     * Counts REAL (wall-clock) time, even while the process sleeps.
     * On expiry the kernel sends SIGALRM.
     * it_value    = time until the FIRST expiry  (10 s + 10 us)
     * it_interval = time between later expiries (10 s + 10 us)
     */
    struct itimerval timer;
    timer.it_value.tv_sec = 10;
    timer.it_value.tv_usec = 10;
    timer.it_interval.tv_sec = 10;
    timer.it_interval.tv_usec = 10;

    signal(SIGALRM, handler);

    start = time(NULL);
    if (setitimer(ITIMER_REAL, &timer, NULL) == -1) {
        perror("setitimer failed");
        exit(EXIT_FAILURE);
    }
    printf("[t=%lds] ITIMER_REAL set for 10 s 10 us\n", (long)(time(NULL) - start));

    while (count < 2) {
        pause();                 // sleep until a signal arrives
    }
    return 0;
}

/*
Output:
shrutis@ubuntu:~/SS_HO2/question_53$ gcc question_53a.c -o question_53a
shrutis@ubuntu:~/SS_HO2/question_53$ ./question_53a
[t=0s] ITIMER_REAL set for 10 s 10 us
[t=10s] SIGALRM received (expiry 1)
[t=20s] SIGALRM received (expiry 2)
*/
