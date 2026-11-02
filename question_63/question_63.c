/* Program Number: 63
Student Name: Shruti Sijariya; Register Number: MT2026127
Date: 01-11-2026
Description: Write a program to create an orphan process. Use the kill system call to send
              a SIGKILL signal from the child process to the parent process.
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

int main() {
    /*
     * CONCEPT: Orphan process
     * An orphan is a child whose parent has died.
     * The child kills its own parent with kill(getppid(), SIGKILL).
     * The orphan is then adopted by init (PID 1) or a "subreaper"
     * process (on Ubuntu desktop: systemd --user), so getppid() changes.
     * SIGKILL cannot be caught or ignored.
     */
    pid_t pid = fork();

    if (pid == -1) {
        perror("fork failed");
        exit(EXIT_FAILURE);
    }

    if (pid == 0) {
        // Child
        sleep(1);
        printf("Child  (PID %d): my parent is %d\n", getpid(), getppid());
        printf("Child: sending SIGKILL to parent...\n");
        kill(getppid(), SIGKILL);

        sleep(2);   // give the kernel time to re-parent this process
        printf("Child  (PID %d): I am an orphan now, my new parent is %d\n", getpid(), getppid());
        exit(EXIT_SUCCESS);
    } else {
        // Parent
        printf("Parent (PID %d): created child %d, now waiting forever...\n", getpid(), pid);
        while (1) {
            sleep(1);
        }
    }
    return 0;
}

/*
Output:
shrutis@ubuntu:~/SS_HO2/question_63$ gcc question_63.c -o question_63
shrutis@ubuntu:~/SS_HO2/question_63$ ./question_63
Parent (PID 979): created child 980, now waiting forever...
Child  (PID 980): my parent is 979
Child: sending SIGKILL to parent...
Killed
shrutis@ubuntu:~/SS_HO2/question_63$ Child  (PID 980): I am an orphan now, my new parent is 1
*/
