/* Program Number: 36a
Student Name: Shruti Sijariya; Register Number: MT2026127
Date: 19-10-2026
Description: Create a FIFO file using the mknod system call.
*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int main() {
    /*
     * CONCEPT: Creating FIFO via mknod()
     * mknod() creates file system nodes (files, device files, or FIFOs).
     * To create a FIFO, bitwise-OR S_IFIFO with the desired permission bits (0666).
     * Dev argument is set to 0 since it is not a character or block device.
     */

    const char *fifo_path = "my_mknod_fifo";
    mode_t mode = S_IFIFO | 0666;

    if (mknod(fifo_path, mode, 0) == -1) {
        perror("mknod failed");
        exit(EXIT_FAILURE);
    }

    printf("FIFO created successfully using mknod() system call at: %s\n", fifo_path);
    return 0;
}

/*
Output:
shrutis@ubuntu:~/SS_HO2/question_36$ gcc question_36a.c -o question_36a
shrutis@ubuntu:~/SS_HO2/question_36$ ./question_36a
FIFO created successfully using mknod() system call at: my_mknod_fifo
shrutis@ubuntu:~/SS_HO2/question_36$ ls -l my_mknod_fifo
prw-rw-r-- 1 shrutis shrutis 0 Oct 19 16:51 my_mknod_fifo
*/