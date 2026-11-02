/* Program Number: 42
Student Name: Shruti Sijariya; Register Number: MT2026127
Date: 20-10-2026
Description: Write a program to print information about a message queue using 
              msqid_ds and ipc_perm structures.
*/

#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

int main() {
    /*
     * CONCEPT: Inspecting Message Queue Attributes via msgctl() & IPC_STAT
     * msgctl(msqid, IPC_STAT, &buf) fetches the kernel data structure 
     * associated with the message queue identified by msqid.
     */

    key_t key = ftok(".", 'A');
    if (key == (key_t)-1) {
        perror("ftok failed");
        exit(EXIT_FAILURE);
    }

    // Get message queue identifier (or create if not present)
    int msqid = msgget(key, IPC_CREAT | 0666);
    if (msqid == -1) {
        perror("msgget failed");
        exit(EXIT_FAILURE);
    }

    struct msqid_ds mq_info;

    // Retrieve metadata from kernel
    if (msgctl(msqid, IPC_STAT, &mq_info) == -1) {
        perror("msgctl IPC_STAT failed");
        exit(EXIT_FAILURE);
    }

    printf("============================================================\n");
    printf("         System V Message Queue Attributes (ID: %d)          \n", msqid);
    printf("============================================================\n");

    // 1. Access permissions
    printf("Access Permissions                  : 0%o\n", mq_info.msg_perm.mode & 0777);

    // 2. UID and GID
    printf("Owner UID                           : %d\n", mq_info.msg_perm.uid);
    printf("Owner GID                           : %d\n", mq_info.msg_perm.gid);

    // 3. Time of last message sent and received
    printf("Time of Last Message Sent (msg_stime): %s",
           mq_info.msg_stime ? ctime(&mq_info.msg_stime) : "Not yet sent\n");
    printf("Time of Last Message Recv (msg_rtime): %s",
           mq_info.msg_rtime ? ctime(&mq_info.msg_rtime) : "Not yet received\n");

    // 4. Time of last change
    printf("Time of Last Change       (msg_ctime): %s",
           mq_info.msg_ctime ? ctime(&mq_info.msg_ctime) : "N/A\n");

    // 5. Current size of the queue (in bytes) - Portable across macOS and Linux
#if defined(__APPLE__) || !defined(__GLIBC__)
    printf("Current Queue Size (bytes)          : %lu bytes\n", (unsigned long)mq_info.msg_cbytes);
#else
    printf("Current Queue Size (bytes)          : %lu bytes\n", (unsigned long)mq_info.__msg_cbytes);
#endif

    // 6. Number of messages in the queue
    printf("Number of Messages in Queue         : %lu\n", (unsigned long)mq_info.msg_qnum);

    // 7. Maximum number of bytes allowed
    printf("Maximum Bytes Allowed (msg_qbytes)  : %lu bytes\n", (unsigned long)mq_info.msg_qbytes);

    // 8. PID of msgsnd and msgrcv
    printf("PID of Last msgsnd Process          : %d\n", mq_info.msg_lspid);
    printf("PID of Last msgrcv Process          : %d\n", mq_info.msg_lrpid);
    printf("============================================================\n");

    return 0;
}

/*
Output:
shrutis@ubuntu:~/SS_HO2/question_42$ gcc question_42.c -o question_42
shrutis@ubuntu:~/SS_HO2/question_42$ ./question_42
============================================================
         System V Message Queue Attributes (ID: 0)          
============================================================
Access Permissions                  : 0666
Owner UID                           : 1000
Owner GID                           : 1000
Time of Last Message Sent (msg_stime): Not yet sent
Time of Last Message Recv (msg_rtime): Not yet received
Time of Last Change       (msg_ctime): Tue Oct 20 14:36:10 2026
Current Queue Size (bytes)          : 0 bytes
Number of Messages in Queue         : 0
Maximum Bytes Allowed (msg_qbytes)  : 16384 bytes
PID of Last msgsnd Process          : 0
PID of Last msgrcv Process          : 0
============================================================
*/