/* Program Number: 36b
Student Name: Shruti Sijariya; Register Number: MT2026127
Date: 19-10-2026
Description: Create a FIFO file using the mkfifo library function.
*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>

int main() {
    /*
     * CONCEPT: Creating FIFO via mkfifo()
     * mkfifo() is the standard POSIX interface specifically designed
     * for creating FIFO special files. It internally configures S_IFIFO
     * and passes the mode adjusted by the process umask.
     */

    const char *fifo_path = "my_mkfifo_fifo";
    mode_t mode = 0666;

    if (mkfifo(fifo_path, mode) == -1) {
        perror("mkfifo failed");
        exit(EXIT_FAILURE);
    }

    printf("FIFO created successfully using mkfifo() library function at: %s\n", fifo_path);
    return 0;
}

/*
Output:
shrutis@ubuntu:~/SS_HO2/question_36$ gcc question_36b.c -o question_36b
shrutis@ubuntu:~/SS_HO2/question_36$ ./question_36b
FIFO created successfully using mkfifo() library function at: my_mkfifo_fifo
shrutis@ubuntu:~/SS_HO2/question_36$ ls -l my_mkfifo_fifo
prw-rw-r-- 1 shrutis shrutis 0 Oct 19 16:52 my_mkfifo_fifo
*/