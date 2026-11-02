/* Program Number: 34a
Student Name: Shruti Sijariya; Register Number: MT2026127
Date: 01-11-2026
Description: Write a program to execute ls -l | wc using:
              a. dup
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

int main() {
    /*
     * CONCEPT: dup(oldfd)
     * dup() returns a copy of oldfd using the LOWEST unused fd number.
     * Trick: close(1) first -> fd 1 is now the lowest free number,
     * so dup(pipefd[1]) returns 1. Now stdout of "ls -l" goes into the pipe.
     * Same for stdin: close(0), then dup(pipefd[0]) returns 0.
     */
    int pipefd[2];

    if (pipe(pipefd) == -1) {
        perror("pipe failed");
        exit(EXIT_FAILURE);
    }

    pid_t pid = fork();
    if (pid == -1) {
        perror("fork failed");
        exit(EXIT_FAILURE);
    }

    if (pid == 0) {
        // Child: runs "ls -l", its stdout must go to the pipe's write end
        close(pipefd[0]);           // child does not read
        close(STDOUT_FILENO);       // free fd 1
        dup(pipefd[1]);             // lowest free fd = 1 -> stdout is now the pipe
        close(pipefd[1]);           // extra copy not needed

        execlp("ls", "ls", "-l", (char *)NULL);
        perror("execlp ls failed");
        exit(EXIT_FAILURE);
    } else {
        // Parent: runs "wc", its stdin must come from the pipe's read end
        close(pipefd[1]);           // parent does not write (needed for EOF)
        close(STDIN_FILENO);        // free fd 0
        dup(pipefd[0]);             // lowest free fd = 0 -> stdin is now the pipe
        close(pipefd[0]);

        execlp("wc", "wc", (char *)NULL);
        perror("execlp wc failed");
        exit(EXIT_FAILURE);
    }
}

/*
Output:
shrutis@ubuntu:~/SS_HO2/question_34$ gcc question_34a.c -o question_34a
shrutis@ubuntu:~/SS_HO2/question_34$ ./question_34a
      7      56     381
shrutis@ubuntu:~/SS_HO2/question_34$ ls -l | wc
      7      56     381
*/
