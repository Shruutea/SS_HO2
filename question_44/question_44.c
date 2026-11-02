/* Program Number: 44
Student Name: Shruti Sijariya; Register Number: MT2026127
Date: 20-10-2026
Description: Write a program to receive messages from the message queue:
             a. With 0 as a flag
             b. With IPC_NOWAIT as a flag
*/

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

/*
 * CONCEPT: Receiving Messages via msgrcv()
 * ssize_t msgrcv(int msqid, void *msgp, size_t msgsz, long msgtyp, int msgflg);
 * 
 * Parameters:
 *   - msgtyp:
 *       0  -> Reads the very first message available on the queue regardless of type.
 *       >0 -> Reads the first message whose mtype matches msgtyp.
 *   - msgflg:
 *       0          -> Blocking mode. If no message matching msgtyp is present,
 *                     the calling process blocks until a matching message arrives.
 *       IPC_NOWAIT -> Non-blocking mode. If no matching message is present,
 *                     msgrcv() returns -1 immediately and sets errno to ENOMSG.
 */

struct message_buffer {
    long mtype;
    char mtext[128];
};

int main() {
    key_t key = ftok(".", 'A');
    if (key == (key_t)-1) {
        perror("ftok failed");
        exit(EXIT_FAILURE);
    }

    // Access the message queue (created previously)
    int msqid = msgget(key, 0666);
    if (msqid == -1) {
        perror("msgget failed (Make sure a message queue exists first via Q41/Q43)");
        exit(EXIT_FAILURE);
    }

    struct message_buffer msg;
    int choice;
    long req_type = 0; // 0 matches any message type

    printf("============================================================\n");
    printf("         System V Message Queue Receiver (ID: %d)            \n", msqid);
    printf("============================================================\n");
    printf("Select Reception Flag Mode:\n");
    printf("  1. Receive with 0 (Blocking wait until message arrives)\n");
    printf("  2. Receive with IPC_NOWAIT (Non-blocking check)\n");
    printf("Enter choice (1 or 2): ");
    if (scanf("%d", &choice) != 1) {
        fprintf(stderr, "Invalid input choice.\n");
        exit(EXIT_FAILURE);
    }

    printf("Enter message type to receive (0 for any type): ");
    if (scanf("%ld", &req_type) != 1) {
        fprintf(stderr, "Invalid type input.\n");
        exit(EXIT_FAILURE);
    }

    if (choice == 1) {
        // --- Mode A: Blocking (flag = 0) ---
        printf("\n[Flag: 0] Waiting for message (process will block until available)...\n");
        ssize_t bytes_received = msgrcv(msqid, &msg, sizeof(msg.mtext), req_type, 0);

        if (bytes_received == -1) {
            perror("msgrcv failed");
            exit(EXIT_FAILURE);
        }

        printf("Message received successfully!\n");
        printf("  Message Type : %ld\n", msg.mtype);
        printf("  Payload Size : %zd bytes\n", bytes_received);
        printf("  Content      : \"%s\"\n", msg.mtext);

    } else if (choice == 2) {
        // --- Mode B: Non-Blocking (flag = IPC_NOWAIT) ---
        printf("\n[Flag: IPC_NOWAIT] Checking for message non-blockingly...\n");
        ssize_t bytes_received = msgrcv(msqid, &msg, sizeof(msg.mtext), req_type, IPC_NOWAIT);

        if (bytes_received == -1) {
            if (errno == ENOMSG) {
                printf("No message of type %ld available in the queue right now (ENOMSG).\n", req_type);
            } else {
                perror("msgrcv error");
                exit(EXIT_FAILURE);
            }
        } else {
            printf("Message received successfully!\n");
            printf("  Message Type : %ld\n", msg.mtype);
            printf("  Payload Size : %zd bytes\n", bytes_received);
            printf("  Content      : \"%s\"\n", msg.mtext);
        }
    } else {
        printf("Invalid choice selected.\n");
    }

    return 0;
}

/*
Output Scenario 1 (Flag 0 - Reading message sent from Q43):
shrutis@ubuntu:~/SS_HO2/question_44$ gcc question_44.c -o question_44
shrutis@ubuntu:~/SS_HO2/question_44$ ./question_44
============================================================
         System V Message Queue Receiver (ID: 0)            
============================================================
Select Reception Flag Mode:
  1. Receive with 0 (Blocking wait until message arrives)
  2. Receive with IPC_NOWAIT (Non-blocking check)
Enter choice (1 or 2): 1
Enter message type to receive (0 for any type): 0

[Flag: 0] Waiting for message (process will block until available)...
Message received successfully!
  Message Type : 1
  Payload Size : 42 bytes
  Content      : "Hello System V Message Queue from Shruti!"

Output Scenario 2 (Flag IPC_NOWAIT - Queue is empty):
shrutis@ubuntu:~/SS_HO2/question_44$ ./question_44
============================================================
         System V Message Queue Receiver (ID: 0)            
============================================================
Select Reception Flag Mode:
  1. Receive with 0 (Blocking wait until message arrives)
  2. Receive with IPC_NOWAIT (Non-blocking check)
Enter choice (1 or 2): 2
Enter message type to receive (0 for any type): 0

[Flag: IPC_NOWAIT] Checking for message non-blockingly...
No message of type 0 available in the queue right now (ENOMSG).
*/