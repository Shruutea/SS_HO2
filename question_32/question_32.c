/* Program Number: 32
Student Name: Shruti Sijariya; Register Number: MT2026127
Date: 19-10-2026
Description: Develop a simple program to send data from the parent process to the child process.
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

int main() {
    /*
     * CONCEPT: Inter-Process Communication via Unidirectional Pipe
     * A pipe provides a half-duplex communication channel between related processes.
     * pipefd[0] is designated for reading, and pipefd[1] is designated for writing.
     * When fork() is called, the file descriptors are duplicated into the child's table.
     * To transfer data from Parent to Child:
     *   - Parent: closes the read end (pipefd[0]) and writes to pipefd[1].
     *   - Child:  closes the write end (pipefd[1]) and reads from pipefd[0].
     * Closing unused ends is necessary to signal EOF properly and prevent deadlocks.
     */

    int pipefd[2];
    pid_t pid;
    char write_buf[] = "Hello from Parent: Operating Systems Lab Exercise 32!";
    char read_buf[128];

    // Create the pipe
    if (pipe(pipefd) == -1) {
        perror("Pipe creation failed");
        exit(EXIT_FAILURE);
    }

    pid = fork();

    if (pid < 0) {
        perror("Fork failed");
        exit(EXIT_FAILURE);
    }

    if (pid > 0) {
        // --- PARENT PROCESS ---
        close(pipefd[0]); // Close unused read end

        printf("[Parent PID: %d] Sending data to Child...\n", getpid());
        write(pipefd[1], write_buf, strlen(write_buf) + 1);

        close(pipefd[1]); // Close write end to indicate end of transmission
        wait(NULL);       // Wait for the child process to terminate
    } else {
        // --- CHILD PROCESS ---
        close(pipefd[1]); // Close unused write end

        read(pipefd[0], read_buf, sizeof(read_buf));
        printf("[Child PID: %d] Received data from Parent: \"%s\"\n", getpid(), read_buf);

        close(pipefd[0]); // Close read end after reading
        exit(EXIT_SUCCESS);
    }

    return 0;
}

/*
Output:
shrutis@ubuntu:~/SS_HO2/question_32$ gcc question_32.c -o question_32
shrutis@ubuntu:~/SS_HO2/question_32$ ./question_32
[Parent PID: 5041] Sending data to Child...
[Child PID: 5042] Received data from Parent: "Hello from Parent: Operating Systems Lab Exercise 32!"
*/