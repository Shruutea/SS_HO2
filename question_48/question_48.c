/* Program Number: 48
Student Name: Shruti Sijariya; Register Number: MT2026127
Date: 20-10-2026
Description: Write a program to create a semaphore and initialize its value:
             a. Create a binary semaphore
             b. Create a counting semaphore
*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>

/*
 * CONCEPT: System V Semaphores Initialization
 * 1. semget(key, nsems, semflg): Allocates a set of semaphores.
 * 2. semctl(semid, semnum, cmd, arg): Performs control operations.
 *    - cmd = SETVAL initializes the semaphore value.
 *    - cmd = GETVAL reads back the current value.
 *
 * Types of Semaphores:
 *   - Binary Semaphore: Initialized to 1 (used for mutual exclusion / mutex).
 *   - Counting Semaphore: Initialized to N > 1 (used to track a pool of resources).
 *
 * The union semun must be explicitly defined by the programmer as required by SUSv2.
 */

union semun {
    int val;                    /* Value for SETVAL */
    struct semid_ds *buf;       /* Buffer for IPC_STAT, IPC_SET */
    unsigned short *array;      /* Array for GETALL, SETALL */
    struct seminfo *__buf;      /* Buffer for IPC_INFO */
};

int main() {
    key_t key_bin = ftok(".", 'B');
    key_t key_count = ftok(".", 'C');

    if (key_bin == (key_t)-1 || key_count == (key_t)-1) {
        perror("ftok failed");
        exit(EXIT_FAILURE);
    }

    union semun arg;

    printf("============================================================\n");
    printf("         System V Semaphore Creation and Initialization     \n");
    printf("============================================================\n");

    // --- Part A: Create and Initialize Binary Semaphore ---
    int semid_bin = semget(key_bin, 1, IPC_CREAT | 0666);
    if (semid_bin == -1) {
        perror("semget binary failed");
        exit(EXIT_FAILURE);
    }

    arg.val = 1; // Binary semaphore initialized to 1 (available)
    if (semctl(semid_bin, 0, SETVAL, arg) == -1) {
        perror("semctl SETVAL binary failed");
        exit(EXIT_FAILURE);
    }

    int val_bin = semctl(semid_bin, 0, GETVAL);
    printf("a. Binary Semaphore:\n");
    printf("   Sem ID           : %d\n", semid_bin);
    printf("   Initialized Value: %d (0 = Locked, 1 = Free)\n\n", val_bin);

    // --- Part B: Create and Initialize Counting Semaphore ---
    int semid_count = semget(key_count, 1, IPC_CREAT | 0666);
    if (semid_count == -1) {
        perror("semget counting failed");
        exit(EXIT_FAILURE);
    }

    int initial_resources = 5; // Counting semaphore initialized to 5 available resources
    arg.val = initial_resources;
    if (semctl(semid_count, 0, SETVAL, arg) == -1) {
        perror("semctl SETVAL counting failed");
        exit(EXIT_FAILURE);
    }

    int val_count = semctl(semid_count, 0, GETVAL);
    printf("b. Counting Semaphore:\n");
    printf("   Sem ID           : %d\n", semid_count);
    printf("   Initialized Value: %d (Available resource units: %d)\n", val_count, initial_resources);
    printf("============================================================\n");
    printf("Verify with: ipcs -s\n");

    return 0;
}

/*
Output:
shrutis@ubuntu:~/SS_HO2/question_48$ gcc question_48.c -o question_48
shrutis@ubuntu:~/SS_HO2/question_48$ ./question_48
============================================================
         System V Semaphore Creation and Initialization     
============================================================
a. Binary Semaphore:
   Sem ID           : 0
   Initialized Value: 1 (0 = Locked, 1 = Free)

b. Counting Semaphore:
   Sem ID           : 1
   Initialized Value: 5 (Available resource units: 5)
============================================================
Verify with: ipcs -s

shrutis@ubuntu:~/SS_HO2/question_48$ ipcs -s

------ Semaphore Arrays --------
key        semid      owner      perms      nsems     
0x42052341 0          shrutis    666        1         
0x43052341 1          shrutis    666        1         
*/