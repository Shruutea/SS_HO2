/* Program Number: 46
Student Name: Shruti Sijariya; Register Number: MT2026127
Date: 20-10-2026
Description: Write a program to remove the message queue.
*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

int main() {
    /*
     * CONCEPT: Removing System V Message Queue via IPC_RMID
     * The msgctl() system call with the IPC_RMID flag immediately removes
     * the message queue identifier and its associated data structure from
     * kernel memory.
     * 
     * Key points:
     *   - Any messages remaining inside the queue are destroyed.
     *   - Any processes blocked in msgsnd() or msgrcv() immediately fail,
     *     returning -1 with errno set to EIDRM (Identifier removed).
     *   - The third argument to msgctl() can safely be set to NULL for IPC_RMID.
     */

    key_t key = ftok(".", 'A');
    if (key == (key_t)-1) {
        perror("ftok failed");
        exit(EXIT_FAILURE);
    }

    // Access the message queue (or create one to ensure there is something to remove)
    int msqid = msgget(key, 0); // open existing queue only (do not create)
    if (msqid == -1) {
        perror("msgget failed");
        exit(EXIT_FAILURE);
    }

    printf("============================================================\n");
    printf("         System V Message Queue Removal (ID: %d)             \n", msqid);
    printf("============================================================\n");

    // Remove the message queue from kernel memory
    if (msgctl(msqid, IPC_RMID, NULL) == -1) {
        perror("msgctl IPC_RMID failed");
        exit(EXIT_FAILURE);
    }

    printf("Message queue with ID %d successfully removed.\n", msqid);
    printf("============================================================\n");
    printf("You can verify removal using: ipcs -q\n");

    return 0;
}

/*
Output:
shrutis@ubuntu:~/SS_HO2/question_46$ gcc question_46.c -o question_46
shrutis@ubuntu:~/SS_HO2/question_46$ ./question_46
============================================================
         System V Message Queue Removal (ID: 0)             
============================================================
Message queue with ID 0 successfully removed.
============================================================
You can verify removal using: ipcs -q

shrutis@ubuntu:~/SS_HO2/question_46$ ipcs -q

------ Message Queues --------
key        msqid      owner      perms      used-bytes   messages    

(Notice that Queue 0 is completely gone from the list)
*/