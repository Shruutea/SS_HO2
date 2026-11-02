/* Program Number: 56
Student Name: Shruti Sijariya; Register Number: MT2026127
Date: 01-11-2026
Description: Write a program to measure the time taken to execute 100 getppid() system calls
              using a time stamp counter.
*/

#include <stdio.h>
#include <unistd.h>

unsigned long long rdtsc() {
    unsigned int lo, hi;
    __asm__ __volatile__ ("rdtsc" : "=a" (lo), "=d" (hi));
    return ((unsigned long long)hi << 32) | lo;
}

int main() {
    /*
     * CONCEPT: Time Stamp Counter (TSC)
     * The CPU has a 64-bit counter that increases every clock cycle.
     * The rdtsc instruction reads it: lower 32 bits in EAX, upper in EDX.
     * cycles = end - start.  time = cycles / CPU frequency.
     * Example: on a 2.8 GHz CPU, 2800 cycles = 1 microsecond.
     */
    unsigned long long start, end;

    start = rdtsc();
    for (int i = 0; i < 100; i++) {
        getppid();
    }
    end = rdtsc();

    printf("Total cycles for 100 getppid() calls : %llu\n", end - start);
    printf("Average cycles per getppid() call    : %llu\n", (end - start) / 100);
    return 0;
}

/*
Output:
shrutis@ubuntu:~/SS_HO2/question_56$ gcc question_56.c -o question_56
shrutis@ubuntu:~/SS_HO2/question_56$ ./question_56
Total cycles for 100 getppid() calls : 30112
Average cycles per getppid() call    : 301
*/
