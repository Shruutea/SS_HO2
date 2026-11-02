/* Program Number: 49c
Student Name: Shruti Sijariya; Register Number: MT2026127
Date: 01-11-2026
Description: Write a program to implement a semaphore to protect a critical section:
              c. Protect multiple pseudo resources (two) using a counting semaphore.
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/wait.h>

#define NUM_RESOURCES 2
#define NUM_PROCESSES 4

union semun {
    int val;
    struct semid_ds *buf;
    unsigned short *array;
};

int main() {
    /*
     * CONCEPT: Counting semaphore
     * Initial value = number of identical resources (2).
     * Each process does P (-1) before using a resource and V (+1) after.
     * When both resources are taken (value 0), the next process blocks.
     * So at most 2 processes are inside at the same time.
     */
    int semid = semget(IPC_PRIVATE, 1, IPC_CREAT | 0666);
    if (semid == -1) {
        perror("semget failed");
        exit(EXIT_FAILURE);
    }

    union semun arg;
    arg.val = NUM_RESOURCES;
    semctl(semid, 0, SETVAL, arg);

    time_t start = time(NULL);

    for (int i = 1; i <= NUM_PROCESSES; i++) {
        if (fork() == 0) {
            struct sembuf p_op = {0, -1, SEM_UNDO};
            struct sembuf v_op = {0, 1, SEM_UNDO};

            semop(semid, &p_op, 1);                       // acquire a resource
            printf("[t=%lds] Process %d (PID %d) got a resource\n",
                   (long)(time(NULL) - start), i, getpid());
            fflush(stdout);
            sleep(2);                                     // use the resource
            printf("[t=%lds] Process %d releasing resource\n",
                   (long)(time(NULL) - start), i);
            fflush(stdout);
            semop(semid, &v_op, 1);                       // release it
            exit(EXIT_SUCCESS);
        }
    }

    for (int i = 0; i < NUM_PROCESSES; i++) {
        wait(NULL);
    }

    semctl(semid, 0, IPC_RMID);
    printf("All processes done. Semaphore removed.\n");
    return 0;
}

/*
Output:
shrutis@ubuntu:~/SS_HO2/question_49$ gcc question_49c.c -o question_49c
shrutis@ubuntu:~/SS_HO2/question_49$ ./question_49c
[t=0s] Process 1 (PID 606) got a resource
[t=0s] Process 2 (PID 607) got a resource
[t=2s] Process 2 releasing resource
[t=2s] Process 1 releasing resource
[t=2s] Process 3 (PID 608) got a resource
[t=2s] Process 4 (PID 609) got a resource
[t=4s] Process 3 releasing resource
[t=4s] Process 4 releasing resource
All processes done. Semaphore removed.
*/
