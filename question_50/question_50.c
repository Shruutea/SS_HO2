/* Program Number: 50
Student Name: Shruti Sijariya; Register Number: MT2026127
Date: 20-10-2026
Description: Write a program to create a deadlock using semaphores.
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/wait.h>

/*
 * CONCEPT: Deadlock using Two Semaphores (Circular Wait)
 * Two semaphores (Resource 0 and Resource 1) are both initialized to 1.
 * 
 * Process 1 (Parent):
 *   1. Locks Resource 0.
 *   2. Sleeps briefly to let Process 2 execute.
 *   3. Attempts to lock Resource 1 (blocks because Process 2 holds it).
 *
 * Process 2 (Child):
 *   1. Locks Resource 1.
 *   2. Sleeps briefly to let Process 1 execute.
 *   3. Attempts to lock Resource 0 (blocks because Process 1 holds it).
 *
 * This fulfills the four Coffman conditions for deadlock:
 *   - Mutual Exclusion: Semaphores hold binary exclusive locks.
 *   - Hold and Wait: Each process holds one lock while waiting for the other.
 *   - No Preemption: Locks cannot be forcibly taken back.
 *   - Circular Wait: P1 waits for P2, and P2 waits for P1.
 */

union semun {
    int val;
    struct semid_ds *buf;
    unsigned short *array;
};

void sem_wait_op(int semid, int sem_num) {
    struct sembuf op = { (unsigned short)sem_num, -1, 0 };
    if (semop(semid, &op, 1) == -1) {
        perror("semop wait failed");
        exit(EXIT_FAILURE);
    }
}

void sem_signal_op(int semid, int sem_num) {
    struct sembuf op = { (unsigned short)sem_num, 1, 0 };
    if (semop(semid, &op, 1) == -1) {
        perror("semop signal failed");
        exit(EXIT_FAILURE);
    }
}

int main() {
    key_t key = ftok(".", 'D');
    if (key == (key_t)-1) {
        perror("ftok failed");
        exit(EXIT_FAILURE);
    }

    // Create a semaphore set with 2 semaphores (index 0 and index 1)
    int semid = semget(key, 2, IPC_CREAT | 0666);
    if (semid == -1) {
        perror("semget failed");
        exit(EXIT_FAILURE);
    }

    // Initialize both semaphores to 1 (available)
    union semun arg;
    arg.val = 1;
    semctl(semid, 0, SETVAL, arg);
    semctl(semid, 1, SETVAL, arg);

    printf("============================================================\n");
    printf("         Deadlock Demonstration using System V Semaphores   \n");
    printf("============================================================\n");
    printf("Semaphore 0 and Semaphore 1 initialized to 1.\n\n");

    pid_t pid = fork();

    if (pid < 0) {
        perror("fork failed");
        exit(EXIT_FAILURE);
    }

    if (pid > 0) {
        // --- PARENT PROCESS ---
        printf("[Parent PID: %d] Step 1: Attempting to lock Semaphore 0...\n", getpid());
        sem_wait_op(semid, 0);
        printf("[Parent PID: %d] Acquired lock on Semaphore 0!\n", getpid());

        // Sleep to ensure child has enough time to acquire Semaphore 1
        sleep(2);

        printf("[Parent PID: %d] Step 2: Attempting to lock Semaphore 1 (Will wait)...\n", getpid());
        sem_wait_op(semid, 1);
        printf("[Parent PID: %d] Acquired lock on Semaphore 1!\n", getpid());

        // Critical section (unreachable due to deadlock)
        printf("[Parent PID: %d] Inside critical section.\n", getpid());

        sem_signal_op(semid, 1);
        sem_signal_op(semid, 0);
        wait(NULL);
    } else {
        // --- CHILD PROCESS ---
        printf("[Child PID: %d] Step 1: Attempting to lock Semaphore 1...\n", getpid());
        sem_wait_op(semid, 1);
        printf("[Child PID: %d] Acquired lock on Semaphore 1!\n", getpid());

        // Sleep to ensure parent has acquired Semaphore 0
        sleep(2);

        printf("[Child PID: %d] Step 2: Attempting to lock Semaphore 0 (Will wait)...\n", getpid());
        sem_wait_op(semid, 0);
        printf("[Child PID: %d] Acquired lock on Semaphore 0!\n", getpid());

        // Critical section (unreachable due to deadlock)
        printf("[Child PID: %d] Inside critical section.\n", getpid());

        sem_signal_op(semid, 0);
        sem_signal_op(semid, 1);
        exit(EXIT_SUCCESS);
    }

    // Cleanup (in practice, terminated via Ctrl+C due to deadlock)
    semctl(semid, 0, IPC_RMID);
    return 0;
}

/*
Output:
shrutis@ubuntu:~/SS_HO2/question_50$ gcc question_50.c -o question_50
shrutis@ubuntu:~/SS_HO2/question_50$ ./question_50
============================================================
         Deadlock Demonstration using System V Semaphores   
============================================================
Semaphore 0 and Semaphore 1 initialized to 1.

[Parent PID: 9012] Step 1: Attempting to lock Semaphore 0...
[Parent PID: 9012] Acquired lock on Semaphore 0!
[Child PID: 9013] Step 1: Attempting to lock Semaphore 1...
[Child PID: 9013] Acquired lock on Semaphore 1!
[Parent PID: 9012] Step 2: Attempting to lock Semaphore 1 (Will wait)...
[Child PID: 9013] Step 2: Attempting to lock Semaphore 0 (Will wait)...

(Both processes are now hung waiting for each other indefinitely - press Ctrl+C to terminate)
*/