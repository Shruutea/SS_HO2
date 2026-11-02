/* Program Number: 49a
Student Name: Shruti Sijariya; Register Number: MT2026127
Date: 20-10-2026
Description: Write a program to implement a semaphore to protect a critical section:
              a. Rewrite the ticket number creation program using a semaphore.
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>

union semun {
    int val;
    struct semid_ds *buf;
    unsigned short *array;
};

void sem_lock(int semid) {
    struct sembuf op = {0, -1, SEM_UNDO};
    if (semop(semid, &op, 1) == -1) {
        perror("sem_lock failed");
        exit(EXIT_FAILURE);
    }
}

void sem_unlock(int semid) {
    struct sembuf op = {0, 1, SEM_UNDO};
    if (semop(semid, &op, 1) == -1) {
        perror("sem_unlock failed");
        exit(EXIT_FAILURE);
    }
}

int main() {
    /*
     * CONCEPT: Ticket Reservation Mutual Exclusion
     * A binary semaphore guarantees that only one process can enter the critical section
     * to read the current ticket count from the file, increment it, and write it back.
     * SEM_UNDO ensures the kernel releases the lock if a process crashes mid-transaction.
     */

    key_t key = ftok(".", 'T');
    // IPC_EXCL: only the FIRST process creates (and initializes) the semaphore.
    // Later processes just open it, so they never reset a lock someone is holding.
    int semid = semget(key, 1, IPC_CREAT | IPC_EXCL | 0666);
    if (semid != -1) {
        union semun arg;
        arg.val = 1;                      // binary semaphore: 1 = available
        semctl(semid, 0, SETVAL, arg);
    } else {
        semid = semget(key, 1, 0666);     // already exists: open, do NOT reset
        if (semid == -1) {
            perror("semget failed");
            exit(EXIT_FAILURE);
        }
    }

    const char *db_file = "tickets.txt";
    int fd = open(db_file, O_RDWR | O_CREAT, 0666);
    if (fd == -1) {
        perror("open failed");
        exit(EXIT_FAILURE);
    }

    int ticket_no = 0;
    if (read(fd, &ticket_no, sizeof(int)) <= 0) {
        ticket_no = 1000;
        lseek(fd, 0, SEEK_SET);
        write(fd, &ticket_no, sizeof(int));
    }

    printf("[PID %d] Requesting ticket lock...\n", getpid());
    sem_lock(semid);
    printf("[PID %d] Locked! Entering critical section...\n", getpid());

    lseek(fd, 0, SEEK_SET);
    read(fd, &ticket_no, sizeof(int));
    printf("[PID %d] Current Ticket: %d -> Booking next ticket: %d\n", getpid(), ticket_no, ticket_no + 1);
    ticket_no++;

    sleep(2); // Simulate processing time

    lseek(fd, 0, SEEK_SET);
    write(fd, &ticket_no, sizeof(int));

    printf("[PID %d] Ticket reserved! Releasing lock...\n", getpid());
    sem_unlock(semid);

    close(fd);
    return 0;
}

/*
Output:
shrutis@ubuntu:~/SS_HO2/question_49$ gcc question_49a.c -o question_49a

--- Terminal 1 ---
shrutis@ubuntu:~/SS_HO2/question_49$ ./question_49a
[PID 729] Requesting ticket lock...
[PID 729] Locked! Entering critical section...
[PID 729] Current Ticket: 1000 -> Booking next ticket: 1001
[PID 729] Ticket reserved! Releasing lock...

--- Terminal 2 (started while Terminal 1 holds the lock) ---
shrutis@ubuntu:~/SS_HO2/question_49$ ./question_49a
[PID 731] Requesting ticket lock...
[PID 731] Locked! Entering critical section...
[PID 731] Current Ticket: 1001 -> Booking next ticket: 1002
[PID 731] Ticket reserved! Releasing lock...
*/
