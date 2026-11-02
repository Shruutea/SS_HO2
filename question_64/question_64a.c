/* Program Number: 64a
Student Name: Shruti Sijariya; Register Number: MT2026127
Date: 01-11-2026
Description: Create two programs: the first program awaits the SIGSTOP signal, while the second
              program sends the signal using the kill system call. Determine whether the first
              program successfully catches the signal or not.
              (Program 1: tries to catch SIGSTOP)
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

#include <errno.h>
#include <string.h>

void handler(int signo) {
    printf("Caught signal %d\n", signo);   // never runs for SIGSTOP
}

int main() {
    /*
     * CONCEPT: SIGSTOP cannot be caught, blocked or ignored.
     * signal(SIGSTOP, handler) fails and returns SIG_ERR (errno = EINVAL).
     * When SIGSTOP arrives, the kernel stops the process directly.
     * SIGCONT resumes it. SIGKILL and SIGSTOP are the only two such signals.
     */
    if (signal(SIGSTOP, handler) == SIG_ERR) {
        printf("signal(SIGSTOP) failed: %s -> SIGSTOP cannot be caught\n", strerror(errno));
    }

    printf("Program 1 PID: %d (run ./question_64b %d in another terminal)\n", getpid(), getpid());

    for (int i = 1; i <= 15; i++) {
        printf("Program 1 running... %d\n", i);
        fflush(stdout);
        sleep(1);
    }
    return 0;
}

/*
Output:
shrutis@ubuntu:~/SS_HO2/question_64$ gcc question_64a.c -o question_64a
shrutis@ubuntu:~/SS_HO2/question_64$ ./question_64a
signal(SIGSTOP) failed: Invalid argument -> SIGSTOP cannot be caught
Program 1 PID: 991 (run ./question_64b 991 in another terminal)
Program 1 running... 1
Program 1 running... 2
Program 1 running... 3
(--- stopped here for 5 seconds by SIGSTOP from question_64b, resumed by SIGCONT ---)
Program 1 running... 4
Program 1 running... 5
Program 1 running... 6
Program 1 running... 7
Program 1 running... 8
Program 1 running... 9
Program 1 running... 10
Program 1 running... 11
Program 1 running... 12
Program 1 running... 13
Program 1 running... 14
Program 1 running... 15
*/
