/* Program Number: 33
Student Name: Shruti Sijariya; Register Number: MT2026127
Date: 20-10-2026
Description: Write a program to demonstrate two-way (duplex) communication 
              between parent and child processes using two pipes.
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

#define BUFFER_SIZE 128

int main() {
    /*
     * CONCEPT: Full-Duplex IPC using Two Half-Duplex Pipes
     * Standard UNIX pipes created via pipe() are strictly unidirectional:
     *   pipefd[0] = Read end
     *   pipefd[1] = Write end
     *
     * To establish full two-way communication without data collisions or deadlocks:
     *   - Pipe 1 (pipe1): Parent writes (pipe1[1]) -> Child reads (pipe1[0])
     *   - Pipe 2 (pipe2): Child writes (pipe2[1]) -> Parent reads (pipe2[0])
     *
     * Unused descriptors MUST be closed immediately in each process to ensure
     * EOF is correctly delivered when the peer process closes its write end.
     */

    int pipe1[2]; // Parent -> Child
    int pipe2[2]; // Child -> Parent
    char buffer[BUFFER_SIZE];

    if (pipe(pipe1) == -1 || pipe(pipe2) == -1) {
        perror("pipe creation failed");
        exit(EXIT_FAILURE);
    }

    pid_t pid = fork();
    if (pid < 0) {
        perror("fork failed");
        exit(EXIT_FAILURE);
    }

    if (pid == 0) {
        // --- CHILD PROCESS ---
        // Close unused pipe ends
        close(pipe1[1]); // Child doesn't write to pipe1
        close(pipe2[0]); // Child doesn't read from pipe2

        // 1. Read message from Parent via pipe1
        ssize_t bytes_read = read(pipe1[0], buffer, sizeof(buffer) - 1);
        if (bytes_read > 0) {
            buffer[bytes_read] = '\0';
            printf("[Child PID %d] Received from Parent via Pipe 1: \"%s\"\n", getpid(), buffer);
        }

        // 2. Send reply back to Parent via pipe2
        const char *reply = "Hello Parent! Acknowledgment from Child via Pipe 2.";
        printf("[Child PID %d] Sending reply to Parent via Pipe 2...\n", getpid());
        write(pipe2[1], reply, strlen(reply));

        // Clean up remaining descriptors
        close(pipe1[0]);
        close(pipe2[1]);
        exit(EXIT_SUCCESS);
    } else {
        // --- PARENT PROCESS ---
        // Close unused pipe ends
        close(pipe1[0]); // Parent doesn't read from pipe1
        close(pipe2[1]); // Parent doesn't write to pipe2

        // 1. Send initial message to Child via pipe1
        const char *msg = "Hello Child! Instruction from Parent via Pipe 1.";
        printf("[Parent PID %d] Sending message to Child via Pipe 1...\n", getpid());
        write(pipe1[1], msg, strlen(msg));

        // 2. Read response from Child via pipe2
        ssize_t bytes_read = read(pipe2[0], buffer, sizeof(buffer) - 1);
        if (bytes_read > 0) {
            buffer[bytes_read] = '\0';
            printf("[Parent PID %d] Received from Child via Pipe 2: \"%s\"\n", getpid(), buffer);
        }

        // Clean up remaining descriptors
        close(pipe1[1]);
        close(pipe2[0]);

        wait(NULL);
    }

    return 0;
}

/*
Output:
shrutis@ubuntu:~/SS_HO2/question_33$ gcc question_33.c -o question_33
shrutis@ubuntu:~/SS_HO2/question_33$ ./question_33
[Parent PID 12410] Sending message to Child via Pipe 1...
[Child PID 12411] Received from Parent via Pipe 1: "Hello Child! Instruction from Parent via Pipe 1."
[Child PID 12411] Sending reply to Parent via Pipe 2...
[Parent PID 12410] Received from Child via Pipe 2: "Hello Parent! Acknowledgment from Child via Pipe 2."
*/