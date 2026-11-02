/* Program Number: 57
Student Name: Shruti Sijariya; Register Number: MT2026127
Date: 01-11-2026
Description: Write a program to print system limitations for:
              a. Maximum length of arguments in the exec family of functions.
              b. Maximum number of simultaneous processes per user ID.
              c. Number of clock ticks (jiffies) per second.
              d. Maximum number of open files.
              e. Size of a page.
              f. Total number of pages in physical memory.
              g. Number of currently available pages in physical memory.
*/

#include <stdio.h>
#include <unistd.h>

int main() {
    /*
     * CONCEPT: sysconf(name)
     * Returns the value of a system limit or setting at run time.
     * Returns -1 if the limit is not defined (no limit).
     */
    long page_size  = sysconf(_SC_PAGESIZE);
    long phys_pages = sysconf(_SC_PHYS_PAGES);
    long avail_pages = sysconf(_SC_AVPHYS_PAGES);

    printf("a. Max length of exec arguments (ARG_MAX) : %ld bytes\n", sysconf(_SC_ARG_MAX));
    printf("b. Max processes per user (CHILD_MAX)      : %ld\n", sysconf(_SC_CHILD_MAX));
    printf("c. Clock ticks per second (CLK_TCK)        : %ld\n", sysconf(_SC_CLK_TCK));
    printf("d. Max open files (OPEN_MAX)               : %ld\n", sysconf(_SC_OPEN_MAX));
    printf("e. Page size                               : %ld bytes\n", page_size);
    printf("f. Total pages in physical memory          : %ld (%ld MB)\n",
           phys_pages, phys_pages * page_size / (1024 * 1024));
    printf("g. Available pages in physical memory      : %ld (%ld MB)\n",
           avail_pages, avail_pages * page_size / (1024 * 1024));
    return 0;
}

/*
Output:
shrutis@ubuntu:~/SS_HO2/question_57$ gcc question_57.c -o question_57
shrutis@ubuntu:~/SS_HO2/question_57$ ./question_57
a. Max length of exec arguments (ARG_MAX) : 2097152 bytes
b. Max processes per user (CHILD_MAX)      : 15897
c. Clock ticks per second (CLK_TCK)        : 100
d. Max open files (OPEN_MAX)               : 1024
e. Page size                               : 4096 bytes
f. Total pages in physical memory          : 1022924 (3995 MB)
g. Available pages in physical memory      : 958033 (3742 MB)
*/
