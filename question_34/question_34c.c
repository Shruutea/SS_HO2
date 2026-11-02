/* Program Number: 34c
Student Name: Shruti Sijariya; Register Number: MT2026127
Date: 01-11-2026
Description: Write a program to execute ls -l | wc using:
              c. fcntl
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

int main() {
    /*
     * CONCEPT: fcntl(oldfd, F_DUPFD, minfd)
     * F_DUPFD returns a copy of oldfd using the lowest free fd number
     * that is >= minfd. So it works like dup(), but you can choose
     * the starting number. close(1) + fcntl(pipefd[1], F_DUPFD, 1) -> returns 1.
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
        // Child: "ls -l" -> stdout redirected to pipe write end
        close(pipefd[0]);
        close(STDOUT_FILENO);                          // free fd 1
        fcntl(pipefd[1], F_DUPFD, STDOUT_FILENO);      // lowest free fd >= 1 is 1
        close(pipefd[1]);

        execlp("ls", "ls", "-l", (char *)NULL);
        perror("execlp ls failed");
        exit(EXIT_FAILURE);
    } else {
        // Parent: "wc" -> stdin redirected from pipe read end
        close(pipefd[1]);
        close(STDIN_FILENO);                           // free fd 0
        fcntl(pipefd[0], F_DUPFD, STDIN_FILENO);       // lowest free fd >= 0 is 0
        close(pipefd[0]);

        execlp("wc", "wc", (char *)NULL);
        perror("execlp wc failed");
        exit(EXIT_FAILURE);
    }
}

/*
Output:
shrutis@ubuntu:~/SS_HO2/question_34$ gcc question_34c.c -o question_34c
shrutis@ubuntu:~/SS_HO2/question_34$ ./question_34c
      7      56     381
shrutis@ubuntu:~/SS_HO2/question_34$ ls -l | wc
      7      56     381
*/
