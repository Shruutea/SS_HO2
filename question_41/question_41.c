/* Program Number: 41
Student Name: Shruti Sijariya; Register Number: MT2026127
Date: 20-10-2026
Description: Write a program to create a message queue, and print the key and message queue ID.
*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

int main() {
    /*
     * CONCEPT: System V IPC Message Queue Creation
     * 1. ftok(pathname, proj_id):
     *    Generates a unique System V IPC key based on the file's inode 
     *    and device numbers combined with the lower 8 bits of proj_id.
     *    Using "." refers to the current working directory.
     * 2. msgget(key, msgflg):
     *    - IPC_CREAT: Creates the queue if it does not already exist.
     *    - IPC_EXCL: Combined with IPC_CREAT, fails if the queue exists.
     *    - 0666: Read and write permissions for user, group, and others.
     *    - Returns the non-negative message queue identifier (msqid) on success.
     */

    const char *path = ".";
    int project_id = 'A';

    // Generate IPC key
    key_t key = ftok(path, project_id);
    if (key == (key_t)-1) {
        perror("ftok failed");
        exit(EXIT_FAILURE);
    }

    // Create the message queue with read/write permissions
    int msqid = msgget(key, IPC_CREAT | 0666);
    if (msqid == -1) {
        perror("msgget failed");
        exit(EXIT_FAILURE);
    }

    printf("--- System V Message Queue Details ---\n");
    printf("IPC Key (hexadecimal) : 0x%08x\n", key);
    printf("IPC Key (decimal)     : %d\n", key);
    printf("Message Queue ID      : %d\n", msqid);

    return 0;
}

/*
Output:
shrutis@ubuntu:~/SS_HO2/question_41$ gcc question_41.c -o question_41
shrutis@ubuntu:~/SS_HO2/question_41$ ./question_41
--- System V Message Queue Details ---
IPC Key (hexadecimal) : 0x41052341
IPC Key (decimal)     : 1090855745
Message Queue ID      : 0

shrutis@ubuntu:~/SS_HO2/question_41$ ipcs -q

------ Message Queues --------
key        msqid      owner      perms      used-bytes   messages    
0x41052341 0          shrutis    666        0            0           
*/