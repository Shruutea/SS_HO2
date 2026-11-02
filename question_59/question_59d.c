/* Program Number: 59d
Student Name: Shruti Sijariya; Register Number: MT2026127
Date: 01-11-2026
Description: Write separate programs using the signal system call to catch the following signals:
              d. SIGALRM (using the alarm system call)
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

void handler(int signo) {
    printf("Caught SIGALRM (signal %d) from alarm()\n", signo);
}

int main() {
    /*
     * CONCEPT: alarm(seconds)
     * Asks the kernel to send SIGALRM after the given number of seconds
     * (one time only). alarm(0) cancels a pending alarm.
     */
    signal(SIGALRM, handler);

    alarm(3);
    printf("Alarm set for 3 seconds, waiting...\n");
    pause();

    return 0;
}

/*
Output:
shrutis@ubuntu:~/SS_HO2/question_59$ gcc question_59d.c -o question_59d
shrutis@ubuntu:~/SS_HO2/question_59$ ./question_59d
Alarm set for 3 seconds, waiting...
Caught SIGALRM (signal 14) from alarm()
*/
