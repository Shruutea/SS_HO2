/* Program Number: 64b
Student Name: Shruti Sijariya; Register Number: MT2026127
Date: 01-11-2026
Description: Create two programs: the first program awaits the SIGSTOP signal, while the second
              program sends the signal using the kill system call.
              (Program 2: sends SIGSTOP, then SIGCONT)
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

int main(int argc, char *argv[]) {
    /*
     * CONCEPT: kill(pid, signal) sends a signal to another process.
     * SIGSTOP pauses the target; SIGCONT makes it continue.
     */
    if (argc != 2) {
        printf("Usage: %s <pid>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    pid_t pid = atoi(argv[1]);

    if (kill(pid, SIGSTOP) == -1) {
        perror("kill SIGSTOP failed");
        exit(EXIT_FAILURE);
    }
    printf("Program 2: sent SIGSTOP to %d (it is stopped now)\n", pid);

    sleep(5);

    kill(pid, SIGCONT);
    printf("Program 2: sent SIGCONT to %d after 5 seconds (it resumes)\n", pid);
    return 0;
}

/*
Output:
shrutis@ubuntu:~/SS_HO2/question_64$ gcc question_64b.c -o question_64b
shrutis@ubuntu:~/SS_HO2/question_64$ ./question_64b 991
Program 2: sent SIGSTOP to 991 (it is stopped now)
Program 2: sent SIGCONT to 991 after 5 seconds (it resumes)
*/
