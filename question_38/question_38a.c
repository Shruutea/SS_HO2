/* Program Number: 38a
Student Name: Shruti Sijariya; Register Number: MT2026127
Date: 20-10-2026
Description: Write two programs to establish communication through FIFO using two-way communication (Program 1).
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
     * CONCEPT: Two-Way FIFO Communication (Program 1)
     * For full-duplex communication, two named pipes are used:
     *   1. fifo_p1_to_p2: Program 1 writes, Program 2 reads.
     *   2. fifo_p2_to_p1: Program 2 writes, Program 1 reads.
     *
     * Deadlock Prevention:
     * To prevent both programs from blocking indefinitely waiting for
     * each other to open, the opening sequence must be complementary:
     *   - Program 1: opens write pipe first, then read pipe.
     *   - Program 2: opens read pipe first, then write pipe.
     */

    const char *fifo_write = "fifo_p1_to_p2";
    const char *fifo_read  = "fifo_p2_to_p1";

    char write_buf[100];
    char read_buf[100];

    // Create both FIFOs if they don't already exist
    mkfifo(fifo_write, 0666);
    mkfifo(fifo_read, 0666);

    printf("[Program 1 PID: %d] Opening FIFO for writing...\n", getpid());
    int fd_write = open(fifo_write, O_WRONLY);
    if (fd_write == -1) {
        perror("open write failed");
        exit(EXIT_FAILURE);
    }

    printf("[Program 1 PID: %d] Opening FIFO for reading...\n", getpid());
    int fd_read = open(fifo_read, O_RDONLY);
    if (fd_read == -1) {
        perror("open read failed");
        exit(EXIT_FAILURE);
    }

    printf("[Program 1 PID: %d] Communication established!\n\n", getpid());

    // Send message to Program 2
    printf("Enter message to send to Program 2: ");
    if (fgets(write_buf, sizeof(write_buf), stdin) != NULL) {
        write_buf[strcspn(write_buf, "\n")] = '\0'; // remove the newline kept by fgets
        write(fd_write, write_buf, strlen(write_buf) + 1);
        printf("[Program 1] Sent: \"%s\"\n", write_buf);
    }

    // Wait and read reply from Program 2
    printf("[Program 1] Waiting for reply...\n");
    int bytes = read(fd_read, read_buf, sizeof(read_buf));
    if (bytes > 0) {
        printf("[Program 1] Received reply: \"%s\"\n", read_buf);
    }

    // Close descriptors
    close(fd_write);
    close(fd_read);

    // Unlink FIFOs after communication completes
    unlink(fifo_write);
    unlink(fifo_read);

    return 0;
}

/*
Output:
shrutis@ubuntu:~/SS_HO2/question_38$ gcc question_38a.c -o question_38a
shrutis@ubuntu:~/SS_HO2/question_38$ ./question_38a
[Program 1 PID: 7120] Opening FIFO for writing...
[Program 1 PID: 7120] Opening FIFO for reading...
[Program 1 PID: 7120] Communication established!

Enter message to send to Program 2: Hello Program 2, from Program 1!
[Program 1] Sent: "Hello Program 2, from Program 1!"
[Program 1] Waiting for reply...
[Program 1] Received reply: "Hello Program 1, got your message!"
*/