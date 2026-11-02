/* Program Number: 59b
Student Name: Shruti Sijariya; Register Number: MT2026127
Date: 01-11-2026
Description: Write separate programs using the signal system call to catch the following signals:
              b. SIGINT
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

void handler(int signo) {
    printf("\nCaught SIGINT (signal %d): Ctrl+C pressed!\n", signo);
}

int main() {
    /*
     * CONCEPT: SIGINT
     * Sent to the foreground process when the user presses Ctrl+C.
     * Default action: terminate. With a handler, the program
     * runs the handler instead of dying.
     */
    signal(SIGINT, handler);

    printf("PID %d: press Ctrl+C...\n", getpid());
    pause();              // wait until a signal arrives

    printf("Handler finished, program exiting normally.\n");
    return 0;
}

/*
Output:
shrutis@ubuntu:~/SS_HO2/question_59$ gcc question_59b.c -o question_59b
shrutis@ubuntu:~/SS_HO2/question_59$ ./question_59b
PID 945: press Ctrl+C...
^C
Caught SIGINT (signal 2): Ctrl+C pressed!
Handler finished, program exiting normally.
*/
