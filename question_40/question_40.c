/* Program Number: 40
Student Name: Shruti Sijariya; Register Number: MT2026127
Date: 20-10-2026
Description: Write a program to print the maximum number of files that can 
              be opened within a process and the size of a pipe (circular buffer).
*/

#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/resource.h>

int main() {
    /*
     * CONCEPT: Process Resource Limits and Pipe Capacity
     * 1. Maximum number of open files per process:
     *    - Can be retrieved using `sysconf(_SC_OPEN_MAX)`.
     *    - Can also be checked using `getrlimit(RLIMIT_NOFILE, &rlim)`.
     * 2. Size of a pipe (circular buffer):
     *    - POSIX specifies `_PC_PIPE_BUF` via `fpathconf()` / `pathconf()`
     *      as the maximum atomic write limit guaranteed without interleaving.
     *    - On Linux, the actual kernel pipe buffer capacity is queried
     *      using the `fcntl()` operation `F_GETPIPE_SZ`.
     */

    int pipefd[2];
    struct rlimit rlim;

    // --- 1. Maximum Open Files per Process ---
    long open_max_sysconf = sysconf(_SC_OPEN_MAX);

    if (getrlimit(RLIMIT_NOFILE, &rlim) == 0) {
        printf("--- Maximum Number of Open Files ---\n");
        printf("Via sysconf(_SC_OPEN_MAX) : %ld\n", open_max_sysconf);
        printf("Via getrlimit Soft Limit  : %ld\n", (long)rlim.rlim_cur);
        printf("Via getrlimit Hard Limit  : %ld\n\n", (long)rlim.rlim_max);
    } else {
        perror("getrlimit failed");
    }

    // --- 2. Pipe Buffer Size ---
    if (pipe(pipefd) == -1) {
        perror("pipe creation failed");
        exit(EXIT_FAILURE);
    }

    // Atomic write limit to pipe guaranteed by POSIX
    long pipe_buf_atomic = fpathconf(pipefd[0], _PC_PIPE_BUF);

    // Actual capacity of the Linux circular kernel buffer for the pipe
    int pipe_capacity = fcntl(pipefd[0], F_GETPIPE_SZ);

    printf("--- Pipe Buffer Information ---\n");
    printf("Atomic Pipe Buffer Size (fpathconf _PC_PIPE_BUF): %ld bytes\n", pipe_buf_atomic);
    if (pipe_capacity != -1) {
        printf("Total Kernel Pipe Capacity (fcntl F_GETPIPE_SZ) : %d bytes (%d KB)\n",
               pipe_capacity, pipe_capacity / 1024);
    } else {
        perror("fcntl F_GETPIPE_SZ failed");
    }

    close(pipefd[0]);
    close(pipefd[1]);

    return 0;
}

/*
Output:
shrutis@ubuntu:~/SS_HO2/question_40$ gcc question_40.c -o question_40
shrutis@ubuntu:~/SS_HO2/question_40$ ./question_40
--- Maximum Number of Open Files ---
Via sysconf(_SC_OPEN_MAX) : 1024
Via getrlimit Soft Limit  : 1024
Via getrlimit Hard Limit  : 1048576

--- Pipe Buffer Information ---
Atomic Pipe Buffer Size (fpathconf _PC_PIPE_BUF): 4096 bytes
Total Kernel Pipe Capacity (fcntl F_GETPIPE_SZ) : 65536 bytes (64 KB)
*/