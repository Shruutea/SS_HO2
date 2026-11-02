/* Program Number: 37b
Student Name: Shruti Sijariya; Register Number: MT2026127
Date: 19-10-2026
Description: Write two programs to enable communication through FIFO using one-way communication (Reader Process).
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

int main() {
    /*
     * CONCEPT: One-Way FIFO Communication (Reader)
     * Opening a FIFO in O_RDONLY blocks until a writer opens the opposite end.
     * When the writer sends data and closes its descriptor, read() returns 
     * the number of bytes read or 0 to signify End-of-File (EOF).
     */

    const char *fifo_path = "my_pipe_37";
    char buffer[100];

    // Create the FIFO special file if it does not already exist
    mkfifo(fifo_path, 0666);

    printf("[Reader PID: %d] Opening FIFO for reading (waiting for writer)...\n", getpid());
    int fd = open(fifo_path, O_RDONLY);
    if (fd == -1) {
        perror("open failed");
        exit(EXIT_FAILURE);
    }
    printf("[Reader PID: %d] Writer connected! Reading data from FIFO...\n", getpid());

    int bytes_read = read(fd, buffer, sizeof(buffer));
    if (bytes_read > 0) {
        printf("[Reader PID: %d] Received data: %s", getpid(), buffer);
    }

    close(fd);

    // Clean up the FIFO file from disk
    unlink(fifo_path);
    return 0;
}

/*
Output:
shrutis@ubuntu:~/SS_HO2/question_37$ gcc question_37b.c -o question_37b
shrutis@ubuntu:~/SS_HO2/question_37$ ./question_37b
[Reader PID: 6113] Opening FIFO for reading (waiting for writer)...
[Reader PID: 6113] Writer connected! Reading data from FIFO...
[Reader PID: 6113] Received data: Hello from the Writer Program!
*/