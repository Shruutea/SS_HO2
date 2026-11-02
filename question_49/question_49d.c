/* Program Number: 49d
Student Name: Shruti Sijariya; Register Number: MT2026127
Date: 01-11-2026
Description: Write a program to implement a semaphore to protect a critical section:
              d. Remove the created semaphore.
*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/sem.h>

int main() {
    /*
     * CONCEPT: Removing a System V semaphore
     * The semaphore created by question_49a (ftok(".", 'T') in this folder)
     * stays in the system after that program exits.
     * semctl(semid, 0, IPC_RMID) removes the WHOLE semaphore set immediately.
     * Any process blocked on it wakes up with errno = EIDRM.
     * Shell equivalent: ipcrm -s <semid>
     */
    key_t key = ftok(".", 'T');
    if (key == -1) {
        perror("ftok failed");
        exit(EXIT_FAILURE);
    }

    int semid = semget(key, 1, 0);        // open existing only, do not create
    if (semid == -1) {
        perror("semget failed (semaphore does not exist)");
        exit(EXIT_FAILURE);
    }

    printf("Semaphore found. Key: 0x%x, ID: %d, Value: %d\n",
           key, semid, semctl(semid, 0, GETVAL));

    if (semctl(semid, 0, IPC_RMID) == -1) {
        perror("semctl IPC_RMID failed");
        exit(EXIT_FAILURE);
    }

    printf("Semaphore %d removed successfully.\n", semid);
    return 0;
}

/*
Output:
shrutis@ubuntu:~/SS_HO2/question_49$ gcc question_49d.c -o question_49d
shrutis@ubuntu:~/SS_HO2/question_49$ ipcs -s

------ Semaphore Arrays --------
key        semid      owner      perms      nsems     
0x5400c17c 2          shrutis    666        1         

shrutis@ubuntu:~/SS_HO2/question_49$ ./question_49d
Semaphore found. Key: 0x5400c17c, ID: 2, Value: 1
Semaphore 2 removed successfully.
shrutis@ubuntu:~/SS_HO2/question_49$ ipcs -s

------ Semaphore Arrays --------
key        semid      owner      perms      nsems     
*/
