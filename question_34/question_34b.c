/* Program Number: 34b
Student Name: Shruti Sijariya; Register Number: MT2026127
Date: 01-11-2026
Description: Write a program to execute ls -l | wc using:
              b. dup2
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

int main() {
    /*
     * CONCEPT: dup2(oldfd, newfd)
     * dup2 makes newfd point to the same thing as oldfd.
     * If newfd is already open, dup2 closes it first (in one step).
     * So no manual close(1) / close(0) is needed, unlike dup().
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
        dup2(pipefd[1], STDOUT_FILENO);   // fd 1 now refers to the pipe
        close(pipefd[1]);

        execlp("ls", "ls", "-l", (char *)NULL);
        perror("execlp ls failed");
        exit(EXIT_FAILURE);
    } else {
        // Parent: "wc" -> stdin redirected from pipe read end
        close(pipefd[1]);
        dup2(pipefd[0], STDIN_FILENO);    // fd 0 now refers to the pipe
        close(pipefd[0]);

        execlp("wc", "wc", (char *)NULL);
        perror("execlp wc failed");
        exit(EXIT_FAILURE);
    }
}

/*
Output:
shrutis@ubuntu:~/SS_HO2/question_34$ gcc question_34b.c -o question_34b
shrutis@ubuntu:~/SS_HO2/question_34$ ./question_34b
      7      56     381
shrutis@ubuntu:~/SS_HO2/question_34$ ls -l | wc
      7      56     381
*/
