/* Program Number: 60
Student Name: Shruti Sijariya; Register Number: MT2026127
Date: 01-11-2026
Description: Write a program to ignore a SIGINT signal and then reset it to the default action
              using the signal system call.
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

int main() {
    /*
     * CONCEPT: SIG_IGN and SIG_DFL
     * signal(SIGINT, SIG_IGN) -> the kernel discards SIGINT (Ctrl+C does nothing).
     * signal(SIGINT, SIG_DFL) -> back to the default action (terminate).
     */
    signal(SIGINT, SIG_IGN);
    printf("SIGINT is IGNORED for 5 seconds. Press Ctrl+C, nothing happens...\n");
    sleep(5);

    signal(SIGINT, SIG_DFL);
    printf("SIGINT reset to DEFAULT. Press Ctrl+C now to terminate...\n");
    sleep(10);

    printf("Ctrl+C was not pressed, exiting normally.\n");
    return 0;
}

/*
Output:
shrutis@ubuntu:~/SS_HO2/question_60$ gcc question_60.c -o question_60
shrutis@ubuntu:~/SS_HO2/question_60$ ./question_60
SIGINT is IGNORED for 5 seconds. Press Ctrl+C, nothing happens...
^C
SIGINT reset to DEFAULT. Press Ctrl+C now to terminate...
^C
*/
