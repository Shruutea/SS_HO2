/* Program Number: 55
Student Name: Shruti Sijariya; Register Number: MT2026127
Date: 01-11-2026
Description: Write a program to set a system resource limit using the setrlimit system call.
*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/resource.h>

int main() {
    /*
     * CONCEPT: setrlimit()
     * 1. getrlimit() first, so the hard limit is filled in correctly.
     * 2. Change the soft limit and call setrlimit().
     * A normal user can raise the soft limit only up to the hard limit.
     * Raising the hard limit needs root -> otherwise fails with EPERM.
     * The new limit applies only to this process and its children.
     */
    struct rlimit rl;

    getrlimit(RLIMIT_NOFILE, &rl);
    printf("Before : Soft = %ld, Hard = %ld\n", (long)rl.rlim_cur, (long)rl.rlim_max);

    // 1. Set the soft limit to 2048 (allowed: 2048 <= hard limit)
    rl.rlim_cur = 2048;
    if (setrlimit(RLIMIT_NOFILE, &rl) == -1) {
        perror("setrlimit failed");
        exit(EXIT_FAILURE);
    }

    getrlimit(RLIMIT_NOFILE, &rl);
    printf("After  : Soft = %ld, Hard = %ld\n", (long)rl.rlim_cur, (long)rl.rlim_max);

    // 2. Try to raise the hard limit above its current value (needs root)
    rl.rlim_max = rl.rlim_max + 1;
    if (setrlimit(RLIMIT_NOFILE, &rl) == -1) {
        perror("Raising hard limit failed");
    } else {
        printf("Hard limit raised (running as root)\n");
    }

    return 0;
}

/*
Output:
shrutis@ubuntu:~/SS_HO2/question_55$ gcc question_55.c -o question_55
shrutis@ubuntu:~/SS_HO2/question_55$ ./question_55
Before : Soft = 1024, Hard = 20000
After  : Soft = 2048, Hard = 20000
Raising hard limit failed: Operation not permitted
*/
