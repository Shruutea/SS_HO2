/* Program Number: 35
Student Name: Shruti Sijariya; Register Number: MT2026127
Date: 19-10-2026
Description: Write a program to find the total number of directories in the 
              present working directory using ls -l | grep ^d | wc. 
              Implement the solution using only dup2.
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main() {
    /*
     * CONCEPT: Three-Stage Pipeline with Two Pipes (`ls -l | grep ^d | wc`)
     * Pipe 1 connects `ls -l` (write end) to `grep ^d` (read end).
     * Pipe 2 connects `grep ^d` (write end) to `wc` (read end).
     *
     * dup2(oldfd, newfd) is used across child processes to configure:
     *   1. Child 1 (`ls -l`):
     *      - STDOUT redirected to pipe1[1] using dup2
     *   2. Child 2 (`grep ^d`):
     *      - STDIN redirected from pipe1[0] using dup2
     *      - STDOUT redirected to pipe2[1] using dup2
     *   3. Child 3 (`wc`):
     *      - STDIN redirected from pipe2[0] using dup2
     *      - STDOUT remains standard output (console)
     */

    int pipe1[2]; // pipe between ls and grep
    int pipe2[2]; // pipe between grep and wc

    if (pipe(pipe1) == -1 || pipe(pipe2) == -1) {
        perror("Pipe creation failed");
        exit(EXIT_FAILURE);
    }

    // --- First Child: executes `ls -l` ---
    if (fork() == 0) {
        dup2(pipe1[1], STDOUT_FILENO);

        // Close all unused pipe descriptors
        close(pipe1[0]);
        close(pipe1[1]);
        close(pipe2[0]);
        close(pipe2[1]);

        execlp("ls", "ls", "-l", (char *)NULL);
        perror("execlp ls failed");
        exit(EXIT_FAILURE);
    }

    // --- Second Child: executes `grep ^d` ---
    if (fork() == 0) {
        dup2(pipe1[0], STDIN_FILENO);
        dup2(pipe2[1], STDOUT_FILENO);

        // Close all unused pipe descriptors
        close(pipe1[0]);
        close(pipe1[1]);
        close(pipe2[0]);
        close(pipe2[1]);

        execlp("grep", "grep", "^d", (char *)NULL);
        perror("execlp grep failed");
        exit(EXIT_FAILURE);
    }

    // --- Third Child: executes `wc` ---
    if (fork() == 0) {
        dup2(pipe2[0], STDIN_FILENO);

        // Close all unused pipe descriptors
        close(pipe1[0]);
        close(pipe1[1]);
        close(pipe2[0]);
        close(pipe2[1]);

        execlp("wc", "wc", (char *)NULL);
        perror("execlp wc failed");
        exit(EXIT_FAILURE);
    }

    // --- Parent Process ---
    // Close all pipe ends in parent so EOF propagates properly to readers
    close(pipe1[0]);
    close(pipe1[1]);
    close(pipe2[0]);
    close(pipe2[1]);

    // Reap all three child processes
    while (wait(NULL) > 0);

    return 0;
}

/*
Output:
shrutis@ubuntu:~/SS_HO2/question_35$ gcc question_35.c -o question_35
shrutis@ubuntu:~/SS_HO2/question_35$ ./question_35
      4      36     224
*/