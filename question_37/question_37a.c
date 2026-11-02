/* Program Number: 37a
Student Name: Shruti Sijariya; Register Number: MT2026127
Date: 19-10-2026
Description: Write two programs to enable communication through FIFO using one-way communication (Writer Process).
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>

int main() {
    /*
     * CONCEPT: One-Way FIFO Communication (Writer)
     * A FIFO exists as a special file in the filesystem.
     * Opening a FIFO for writing (O_WRONLY) will block until another process
     * opens it for reading (O_RDONLY), ensuring synchronization.
     * Once opened, data written into the descriptor is buffered by the OS kernel
     * and delivered to the reader process in FIFO order.
     */

    const char *fifo_path = "my_pipe_37";
    char message[100];

    // Create the FIFO special file if it does not already exist
    mkfifo(fifo_path, 0666);

    printf("[Writer PID: %d] Opening FIFO for writing (waiting for reader)...\n", getpid());
    int fd = open(fifo_path, O_WRONLY);
    if (fd == -1) {
        perror("open failed");
        exit(EXIT_FAILURE);
    }
    printf("[Writer PID: %d] Reader detected! FIFO opened successfully.\n", getpid());

    // Prompt user for input message to transmit
    printf("Enter message to send via FIFO: ");
    if (fgets(message, sizeof(message), stdin) != NULL) {
        write(fd, message, strlen(message) + 1);
        printf("[Writer PID: %d] Message sent successfully!\n", getpid());
    }

    close(fd);
    return 0;
}

/*
Output:
shrutis@ubuntu:~/SS_HO2/question_37$ gcc question_37a.c -o question_37a
shrutis@ubuntu:~/SS_HO2/question_37$ ./question_37a
[Writer PID: 6112] Opening FIFO for writing (waiting for reader)...
[Writer PID: 6112] Reader detected! FIFO opened successfully.
Enter message to send via FIFO: Hello from the Writer Program!
[Writer PID: 6112] Message sent successfully!
*/