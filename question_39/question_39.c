/* Program Number: 39
Student Name: Shruti Sijariya; Register Number: MT2026127
Date: 20-10-2026
Description: Write a program to wait for data to be written into a FIFO 
              within 10 seconds, utilizing the select system call with the FIFO.
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/select.h>
#include <string.h>

int main() {
    /*
     * CONCEPT: Synchronous I/O Multiplexing via select() on a FIFO
     * 1. A FIFO is opened in non-blocking read mode (O_RDONLY | O_NONBLOCK) 
     *    so that open() does not block indefinitely waiting for a writer.
     * 2. An fd_set readfds set is initialized using FD_ZERO() and FD_SET().
     * 3. A struct timeval timeout is configured for 10 seconds and 0 microseconds.
     * 4. select() monitors the file descriptor:
     *      - Returns > 0 if data becomes available for reading within 10 seconds.
     *      - Returns 0 if the 10-second timer expires before data arrives.
     *      - Returns -1 if an error or interrupting signal occurs.
     */

    const char *fifo_path = "my_select_fifo";
    char buffer[100];
    fd_set readfds;
    struct timeval timeout;

    // Create FIFO special file if not already present
    mkfifo(fifo_path, 0666);

    printf("[PID: %d] Opening FIFO in non-blocking read mode...\n", getpid());
    int fd = open(fifo_path, O_RDONLY | O_NONBLOCK);
    if (fd == -1) {
        perror("open failed");
        exit(EXIT_FAILURE);
    }

    // Initialize the file descriptor set
    FD_ZERO(&readfds);
    FD_SET(fd, &readfds);

    // Set timeout to 10 seconds
    timeout.tv_sec = 10;
    timeout.tv_usec = 0;

    printf("Waiting up to 10 seconds for data in FIFO '%s'...\n", fifo_path);
    printf("(Tip: In another terminal, run: echo \"Hello!\" > %s)\n", fifo_path);

    int retval = select(fd + 1, &readfds, NULL, NULL, &timeout);

    if (retval == -1) {
        perror("select error");
    } else if (retval == 0) {
        printf("Timeout reached! No data was written to the FIFO within 10 seconds.\n");
    } else {
        if (FD_ISSET(fd, &readfds)) {
            int bytes_read = read(fd, buffer, sizeof(buffer) - 1);
            if (bytes_read > 0) {
                buffer[bytes_read] = '\0';
                printf("Data available! Received message: \"%s\"\n", buffer);
            }
        }
    }

    close(fd);
    unlink(fifo_path);
    return 0;
}

/*
Output Scenario 1 (Data written within 10 seconds):
shrutis@ubuntu:~/SS_HO2/question_39$ gcc question_39.c -o question_39
shrutis@ubuntu:~/SS_HO2/question_39$ ./question_39
[PID: 7810] Opening FIFO in non-blocking read mode...
Waiting up to 10 seconds for data in FIFO 'my_select_fifo'...
(Tip: In another terminal, run: echo "Hello!" > my_select_fifo)
Data available! Received message: "Hello!
"

Output Scenario 2 (Timeout - No data written):
shrutis@ubuntu:~/SS_HO2/question_39$ ./question_39
[PID: 7822] Opening FIFO in non-blocking read mode...
Waiting up to 10 seconds for data in FIFO 'my_select_fifo'...
(Tip: In another terminal, run: echo "Hello!" > my_select_fifo)
Timeout reached! No data was written to the FIFO within 10 seconds.
*/