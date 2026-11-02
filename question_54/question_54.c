/* Program Number: 54
Student Name: Shruti Sijariya; Register Number: MT2026127
Date: 01-11-2026
Description: Write a program to print system resource limits using the getrlimit system call.
*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/resource.h>

void print_limit(int resource, const char *name) {
    struct rlimit rl;

    if (getrlimit(resource, &rl) == -1) {
        perror("getrlimit failed");
        return;
    }

    printf("%-18s", name);

    if (rl.rlim_cur == RLIM_INFINITY)
        printf("Soft: %-12s", "unlimited");
    else
        printf("Soft: %-12ld", (long)rl.rlim_cur);

    if (rl.rlim_max == RLIM_INFINITY)
        printf("Hard: %s\n", "unlimited");
    else
        printf("Hard: %ld\n", (long)rl.rlim_max);
}

int main() {
    /*
     * CONCEPT: Resource limits
     * Every process has a soft limit and a hard limit for each resource.
     * Soft limit (rlim_cur): the limit actually enforced.
     * Hard limit (rlim_max): ceiling for the soft limit.
     * RLIM_INFINITY means "no limit".
     */
    print_limit(RLIMIT_CPU,        "CPU (sec)");
    print_limit(RLIMIT_FSIZE,      "FSIZE (bytes)");
    print_limit(RLIMIT_DATA,       "DATA (bytes)");
    print_limit(RLIMIT_STACK,      "STACK (bytes)");
    print_limit(RLIMIT_CORE,       "CORE (bytes)");
    print_limit(RLIMIT_RSS,        "RSS (bytes)");
    print_limit(RLIMIT_NPROC,      "NPROC");
    print_limit(RLIMIT_NOFILE,     "NOFILE");
    print_limit(RLIMIT_MEMLOCK,    "MEMLOCK (bytes)");
    print_limit(RLIMIT_AS,         "AS (bytes)");
    print_limit(RLIMIT_LOCKS,      "LOCKS");
    print_limit(RLIMIT_SIGPENDING, "SIGPENDING");
    print_limit(RLIMIT_MSGQUEUE,   "MSGQUEUE (bytes)");
    print_limit(RLIMIT_NICE,       "NICE");
    print_limit(RLIMIT_RTPRIO,     "RTPRIO");
    return 0;
}

/*
Output:
shrutis@ubuntu:~/SS_HO2/question_54$ gcc question_54.c -o question_54
shrutis@ubuntu:~/SS_HO2/question_54$ ./question_54
CPU (sec)         Soft: unlimited   Hard: unlimited
FSIZE (bytes)     Soft: unlimited   Hard: unlimited
DATA (bytes)      Soft: unlimited   Hard: unlimited
STACK (bytes)     Soft: 8388608     Hard: unlimited
CORE (bytes)      Soft: 0           Hard: unlimited
RSS (bytes)       Soft: unlimited   Hard: unlimited
NPROC             Soft: 15897       Hard: 15897
NOFILE            Soft: 1024        Hard: 20000
MEMLOCK (bytes)   Soft: 8388608     Hard: 8388608
AS (bytes)        Soft: unlimited   Hard: unlimited
LOCKS             Soft: unlimited   Hard: unlimited
SIGPENDING        Soft: 15897       Hard: 15897
MSGQUEUE (bytes)  Soft: 819200      Hard: 819200
NICE              Soft: 0           Hard: 0
RTPRIO            Soft: 0           Hard: 0
*/
