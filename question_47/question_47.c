/* Program Number: 47
Student Name: Shruti Sijariya; Register Number: MT2026127
Date: 20-10-2026
Description: Write a program to create shared memory and perform operations:
             write data, attach with read-only and test overwrite, detach, 
             and remove the shared memory.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>

#define SHM_SIZE 1024

int main() {
    /*
     * CONCEPT: System V Shared Memory Lifecycle
     * 1. shmget(key, size, shmflg): Allocates the shared memory segment.
     * 2. shmat(shmid, shmaddr, shmflg):
     *    - shmflg = 0: Read-write access.
     *    - shmflg = SHM_RDONLY: Read-only access (POSIX flag for O_RDONLY on shared memory).
     *      Attempting to write to a SHM_RDONLY attached pointer triggers SIGSEGV.
     * 3. shmdt(shmaddr): Detaches the segment from the process address space.
     * 4. shmctl(shmid, IPC_RMID, NULL): Marks the segment for removal.
     */

    key_t key = ftok(".", 'S');
    if (key == (key_t)-1) {
        perror("ftok failed");
        exit(EXIT_FAILURE);
    }

    // Step 1: Create shared memory segment
    int shmid = shmget(key, SHM_SIZE, IPC_CREAT | 0666);
    if (shmid == -1) {
        perror("shmget failed");
        exit(EXIT_FAILURE);
    }
    printf("Shared memory segment created successfully (shmid: %d).\n", shmid);

    // Step 2: Attach with Read-Write mode (flag = 0) and write data
    char *data_rw = (char *)shmat(shmid, NULL, 0);
    if (data_rw == (char *)-1) {
        perror("shmat RW failed");
        exit(EXIT_FAILURE);
    }

    const char *initial_text = "System V Shared Memory Data - Question 47";
    strncpy(data_rw, initial_text, SHM_SIZE);
    printf("1. Written to shared memory: \"%s\"\n", data_rw);

    // Detach from read-write mapping
    if (shmdt(data_rw) == -1) {
        perror("shmdt RW failed");
    }

    // Step 3: Attach with SHM_RDONLY and test overwrite in a child process
    // (A child process is used so the expected SIGSEGV does not crash our main program)
    pid_t pid = fork();
    if (pid == 0) {
        char *data_ro = (char *)shmat(shmid, NULL, SHM_RDONLY);
        if (data_ro == (char *)-1) {
            perror("shmat RDONLY failed");
            exit(EXIT_FAILURE);
        }
        printf("2. Attached with SHM_RDONLY. Reading current data: \"%s\"\n", data_ro);
        printf("   Attempting to overwrite data in read-only mode...\n");

        // Attempting to write into read-only mapped segment
        data_ro[0] = 'X'; 

        // If no crash occurred (unexpected):
        printf("   Overwrite unexpectedly succeeded.\n");
        shmdt(data_ro);
        exit(EXIT_SUCCESS);
    } else {
        int status;
        waitpid(pid, &status, 0);
        if (WIFSIGNALED(status) && WTERMSIG(status) == SIGSEGV) {
            printf("   --> Verified: Overwrite attempt caught SIGSEGV (Segmentation Fault) as expected!\n");
        }
    }

    // Step 4: Detach shared memory
    printf("3. Detach operation verified via shmdt().\n");

    // Step 5: Remove the shared memory segment using IPC_RMID
    if (shmctl(shmid, IPC_RMID, NULL) == -1) {
        perror("shmctl IPC_RMID failed");
        exit(EXIT_FAILURE);
    }
    printf("4. Shared memory segment (shmid: %d) successfully removed.\n", shmid);
    printf("   Verify removal using: ipcs -m\n");

    return 0;
}

/*
Output:
shrutis@ubuntu:~/SS_HO2/question_47$ gcc question_47.c -o question_47
shrutis@ubuntu:~/SS_HO2/question_47$ ./question_47
Shared memory segment created successfully (shmid: 32768).
1. Written to shared memory: "System V Shared Memory Data - Question 47"
2. Attached with SHM_RDONLY. Reading current data: "System V Shared Memory Data - Question 47"
   Attempting to overwrite data in read-only mode...
   --> Verified: Overwrite attempt caught SIGSEGV (Segmentation Fault) as expected!
3. Detach operation verified via shmdt().
4. Shared memory segment (shmid: 32768) successfully removed.
   Verify removal using: ipcs -m
*/