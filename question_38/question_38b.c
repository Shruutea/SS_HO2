/* Program Number: 38b
Student Name: Shruti Sijariya; Register Number: MT2026127
Date: 20-10-2026
Description: Write two programs to establish communication through FIFO using two-way communication (Program 2).
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
     * CONCEPT: Two-Way FIFO Communication (Program 2)
     * Reads from fifo_p1_to_p2 and writes to fifo_p2_to_p1.
     * Opens read end first to pair with Program 1 opening write end first.
     */

    const char *fifo_read  = "fifo_p1_to_p2";
    const char *fifo_write = "fifo_p2_to_p1";

    char read_buf[100];
    char write_buf[100];

    // Ensure both FIFOs exist
    mkfifo(fifo_read, 0666);
    mkfifo(fifo_write, 0666);

    printf("[Program 2 PID: %d] Opening FIFO for reading...\n", getpid());
    int fd_read = open(fifo_read, O_RDONLY);
    if (fd_read == -1) {
        perror("open read failed");
        exit(EXIT_FAILURE);
    }

    printf("[Program 2 PID: %d] Opening FIFO for writing...\n", getpid());
    int fd_write = open(fifo_write, O_WRONLY);
    if (fd_write == -1) {
        perror("open write failed");
        exit(EXIT_FAILURE);
    }

    printf("[Program 2 PID: %d] Communication established!\n\n", getpid());

    // Read message from Program 1
    int bytes = read(fd_read, read_buf, sizeof(read_buf));
    if (bytes > 0) {
        printf("[Program 2] Received message: \"%s\"\n", read_buf);
    }

    // Send response back to Program 1
    printf("Enter reply to send back to Program 1: ");
    if (fgets(write_buf, sizeof(write_buf), stdin) != NULL) {
        write_buf[strcspn(write_buf, "\n")] = '\0'; // remove the newline kept by fgets
        write(fd_write, write_buf, strlen(write_buf) + 1);
        printf("[Program 2] Reply sent!\n");
    }

    // Close descriptors
    close(fd_read);
    close(fd_write);

    return 0;
}

/*
Output:
shrutis@ubuntu:~/SS_HO2/question_38$ gcc question_38b.c -o question_38b
shrutis@ubuntu:~/SS_HO2/question_38$ ./question_38b
[Program 2 PID: 7125] Opening FIFO for reading...
[Program 2 PID: 7125] Opening FIFO for writing...
[Program 2 PID: 7125] Communication established!

[Program 2] Received message: "Hello Program 2, from Program 1!"
Enter reply to send back to Program 1: Hello Program 1, got your message!
[Program 2] Reply sent!
*/