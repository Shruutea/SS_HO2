/* Program Number: 31
Student Name: Shruti Sijariya; Register Number: MT2026127
Date: 18-10-2026
Description: Write a simple program to create a pipe, write to the pipe, 
              read from the pipe, and display the content on the monitor.
*/

#include <stdio.h>
#include <unistd.h>
#include <string.h>

int main() {
    /*
     * CONCEPT: Unidirectional Pipe
     * pipefd[0] is the read end, pipefd[1] is the write end.
     */
    int pipefd[2];
    char write_msg[] = "Hello from pipe!\n";
    char read_msg[100];

    if (pipe(pipefd) == -1) {
        perror("pipe failed");
        return 1;
    }

    write(pipefd[1], write_msg, strlen(write_msg));

    int bytes_read = read(pipefd[0], read_msg, sizeof(read_msg) - 1);
    if (bytes_read >= 0) {
        read_msg[bytes_read] = '\0';
    }

    write(STDOUT_FILENO, read_msg, bytes_read); //1: stdout file descriptor <-> STDOUT_FILENO

    close(pipefd[0]);
    close(pipefd[1]);

    return 0;
}

/*
Output:
shrutis@ubuntu:~/SS_HO2/question_31$ gcc question_31.c -o question_31
shrutis@ubuntu:~/SS_HO2/question_31$ ./question_31
Hello from pipe!
*/