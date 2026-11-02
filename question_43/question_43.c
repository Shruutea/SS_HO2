/* Program Number: 43
Student Name: Shruti Sijariya; Register Number: MT2026127
Date: 20-10-2026
Description: Write a program to send messages to the message queue. Check using $ipcs -q.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

/*
 * CONCEPT: System V Message Queue Transmission
 * System V messages are exchanged using a user-defined struct that MUST
 * begin with a `long mtype` field (> 0), followed by the message payload data:
 *
 *   struct msgbuf {
 *       long mtype;       // Message type (must be > 0)
 *       char mtext[size]; // Message body
 *   };
 *
 * The msgsnd() system call:
 *   int msgsnd(int msqid, const void *msgp, size_t msgsz, int msgflg);
 *   - `msgsz`: size of payload ONLY (sizeof(struct msgbuf) - sizeof(long))
 *   - `msgflg`: 0 (blocking wait if queue is full) or IPC_NOWAIT
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

    // Get or create the message queue with read/write permissions
    int msqid = msgget(key, IPC_CREAT | 0666);
    if (msqid == -1) {
        perror("msgget failed");
        exit(EXIT_FAILURE);
    }

    struct message_buffer msg;

    printf("--- System V Message Queue Sender (ID: %d) ---\n", msqid);

    // Prompt for message type (must be a positive integer)
    printf("Enter message type (positive integer): ");
    if (scanf("%ld", &msg.mtype) != 1 || msg.mtype <= 0) {
        fprintf(stderr, "Invalid message type. It must be greater than 0.\n");
        exit(EXIT_FAILURE);
    }
    getchar(); // Clear trailing newline from stdin

    // Prompt for text content
    printf("Enter message text: ");
    if (fgets(msg.mtext, sizeof(msg.mtext), stdin) == NULL) {
        perror("fgets failed");
        exit(EXIT_FAILURE);
    }

    // Strip trailing newline character if present
    msg.mtext[strcspn(msg.mtext, "\n")] = '\0';

    // msgsz is strictly the size of the payload, excluding mtype
    size_t payload_size = strlen(msg.mtext) + 1;

    if (msgsnd(msqid, &msg, payload_size, 0) == -1) {
        perror("msgsnd failed");
        exit(EXIT_FAILURE);
    }

    printf("\nMessage sent successfully!\n");
    printf("  Queue ID     : %d\n", msqid);
    printf("  Message Type : %ld\n", msg.mtype);
    printf("  Payload Size : %zu bytes\n", payload_size);
    printf("  Content      : \"%s\"\n", msg.mtext);
    printf("\nRun 'ipcs -q' in your terminal to inspect the active queue.\n");

    return 0;
}

/*
Output:
shrutis@ubuntu:~/SS_HO2/question_43$ gcc question_43.c -o question_43
shrutis@ubuntu:~/SS_HO2/question_43$ ./question_43
--- System V Message Queue Sender (ID: 0) ---
Enter message type (positive integer): 1
Enter message text: Hello System V Message Queue from Shruti!

Message sent successfully!
  Queue ID     : 0
  Message Type : 1
  Payload Size : 42 bytes
  Content      : "Hello System V Message Queue from Shruti!"

Run 'ipcs -q' in your terminal to inspect the active queue.

shrutis@ubuntu:~/SS_HO2/question_43$ ipcs -q

------ Message Queues --------
key        msqid      owner      perms      used-bytes   messages    
0x41052341 0          shrutis    666        42           1           
*/