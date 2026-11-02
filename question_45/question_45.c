/* Program Number: 45
Student Name: Shruti Sijariya; Register Number: MT2026127
Date: 20-10-2026
Description: Write a program to change the existing message queue permissions using the msqid_ds structure.
*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

int main() {
    /*
     * CONCEPT: Modifying Message Queue Attributes via IPC_SET
     * The msgctl() system call with the IPC_SET flag allows updating 
     * specific fields within the kernel's msqid_ds structure:
     *   - msg_perm.uid
     *   - msg_perm.gid
     *   - msg_perm.mode (permission bits)
     *   - msg_qbytes    (maximum queue capacity in bytes, root only)
     *
     * Procedure:
     *   1. Call msgctl(msqid, IPC_STAT, &ds) to fetch current attributes.
     *   2. Update ds.msg_perm.mode with the new permission mask.
     *   3. Call msgctl(msqid, IPC_SET, &ds) to write changes to kernel.
     */

    key_t key = ftok(".", 'A');
    if (key == (key_t)-1) {
        perror("ftok failed");
        exit(EXIT_FAILURE);
    }

    // Access or create message queue
    int msqid = msgget(key, IPC_CREAT | 0666);
    if (msqid == -1) {
        perror("msgget failed");
        exit(EXIT_FAILURE);
    }

    struct msqid_ds ds;

    // Step 1: Read current queue metadata
    if (msgctl(msqid, IPC_STAT, &ds) == -1) {
        perror("msgctl IPC_STAT failed");
        exit(EXIT_FAILURE);
    }

    printf("============================================================\n");
    printf("     System V Message Queue Permission Modifier (ID: %d)     \n", msqid);
    printf("============================================================\n");
    printf("Current Queue Permissions : 0%o\n", ds.msg_perm.mode & 0777);

    // Step 2: Update permission bits (e.g., change from 0666 to 0644 or 0600)
    mode_t new_mode = 0644; // rw-r--r--
    ds.msg_perm.mode = (ds.msg_perm.mode & ~0777) | new_mode;

    // Step 3: Apply changes using IPC_SET
    if (msgctl(msqid, IPC_SET, &ds) == -1) {
        perror("msgctl IPC_SET failed");
        exit(EXIT_FAILURE);
    }

    printf("Permissions successfully updated to: 0%o\n", new_mode);

    // Step 4: Verify by reading back the updated metadata
    if (msgctl(msqid, IPC_STAT, &ds) == -1) {
        perror("msgctl verification IPC_STAT failed");
        exit(EXIT_FAILURE);
    }
    printf("Verified Queue Permissions: 0%o\n", ds.msg_perm.mode & 0777);
    printf("============================================================\n");
    printf("You can verify with: ipcs -q\n");

    return 0;
}

/*
Output:
shrutis@ubuntu:~/SS_HO2/question_45$ gcc question_45.c -o question_45
shrutis@ubuntu:~/SS_HO2/question_45$ ./question_45
============================================================
     System V Message Queue Permission Modifier (ID: 0)     
============================================================
Current Queue Permissions : 0666
Permissions successfully updated to: 0644
Verified Queue Permissions: 0644
============================================================
You can verify with: ipcs -q

shrutis@ubuntu:~/SS_HO2/question_45$ ipcs -q

------ Message Queues --------
key        msqid      owner      perms      used-bytes   messages    
0x41052341 0          shrutis    644        0            0           
*/