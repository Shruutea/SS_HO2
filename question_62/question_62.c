/* Program Number: 62
Student Name: Shruti Sijariya; Register Number: MT2026127
Date: 01-11-2026
Description: Write a program to ignore a SIGINT signal and then reset it to the default action
              using the sigaction system call.
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

int main() {
    /*
     * CONCEPT: sa_handler = SIG_IGN / SIG_DFL with sigaction
     * Same idea as question 60, but using sigaction().
     */
    struct sigaction sa;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;

    sa.sa_handler = SIG_IGN;
    sigaction(SIGINT, &sa, NULL);
    printf("SIGINT is IGNORED for 5 seconds. Press Ctrl+C, nothing happens...\n");
    sleep(5);

    sa.sa_handler = SIG_DFL;
    sigaction(SIGINT, &sa, NULL);
    printf("SIGINT reset to DEFAULT. Press Ctrl+C now to terminate...\n");
    sleep(10);

    printf("Ctrl+C was not pressed, exiting normally.\n");
    return 0;
}

/*
Output:
shrutis@ubuntu:~/SS_HO2/question_62$ gcc question_62.c -o question_62
shrutis@ubuntu:~/SS_HO2/question_62$ ./question_62
SIGINT is IGNORED for 5 seconds. Press Ctrl+C, nothing happens...
^C
SIGINT reset to DEFAULT. Press Ctrl+C now to terminate...
^C
*/
