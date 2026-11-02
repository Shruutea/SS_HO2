/* Program Number: 49b
Student Name: Shruti Sijariya; Register Number: MT2026127
Date: 01-11-2026
Description: Write a program to implement a semaphore to protect a critical section:
              b. Protect shared memory from concurrent write access.
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/wait.h>

#define ITERATIONS 100000

union semun {
    int val;
    struct semid_ds *buf;
    unsigned short *array;
};

void sem_lock(int semid) {
    struct sembuf op = {0, -1, SEM_UNDO};   // P operation
    semop(semid, &op, 1);
}

void sem_unlock(int semid) {
    struct sembuf op = {0, 1, SEM_UNDO};    // V operation
    semop(semid, &op, 1);
}

int main() {
    /*
     * CONCEPT: Shared memory has NO synchronization of its own.
     * Parent and child both do (*counter)++ on the same shared memory.
     * counter++ is 3 steps (read, add, write), so without a lock,
     * updates get lost. A binary semaphore (value 1) lets only one
     * process into the critical section at a time.
     * Expected final value = 2 * ITERATIONS.
     */
    int shmid = shmget(IPC_PRIVATE, sizeof(int), IPC_CREAT | 0666);
    if (shmid == -1) {
        perror("shmget failed");
        exit(EXIT_FAILURE);
    }

    int semid = semget(IPC_PRIVATE, 1, IPC_CREAT | 0666);
    if (semid == -1) {
        perror("semget failed");
        exit(EXIT_FAILURE);
    }

    union semun arg;
    arg.val = 1;                          // binary semaphore: available
    semctl(semid, 0, SETVAL, arg);

    int *counter = (int *)shmat(shmid, NULL, 0);
    *counter = 0;

    pid_t pid = fork();                   // child inherits the attached memory
    if (pid == -1) {
        perror("fork failed");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < ITERATIONS; i++) {
        sem_lock(semid);
        (*counter)++;                     // critical section
        sem_unlock(semid);
    }

    if (pid == 0) {
        shmdt(counter);
        exit(EXIT_SUCCESS);
    }

    wait(NULL);
    printf("Expected value : %d\n", 2 * ITERATIONS);
    printf("Final value    : %d\n", *counter);

    shmdt(counter);
    shmctl(shmid, IPC_RMID, NULL);        // remove shared memory
    semctl(semid, 0, IPC_RMID);           // remove semaphore
    return 0;
}

/*
Output:
shrutis@ubuntu:~/SS_HO2/question_49$ gcc question_49b.c -o question_49b
shrutis@ubuntu:~/SS_HO2/question_49$ ./question_49b
Expected value : 200000
Final value    : 200000
*/
