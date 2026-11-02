/* Program Number: 58
Student Name: Shruti Sijariya; Register Number: MT2026127
Date: 01-11-2026
Description: Write a simple program to create three threads and print the IDs of the created threads.
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

void *thread_func(void *arg) {
    long num = (long)arg;
    printf("Thread %ld: my ID (pthread_self) = %lu\n", num, (unsigned long)pthread_self());
    return NULL;
}

int main() {
    /*
     * CONCEPT: Creating threads
     * pthread_create(&tid, attr, function, arg):
     *   tid      -> ID of the new thread is stored here
     *   attr     -> NULL = default attributes
     *   function -> the function the thread runs
     *   arg      -> one argument passed to that function
     * pthread_join() waits for a thread to finish (like wait() for processes).
     * Compile with: gcc question_58.c -o question_58 -pthread
     */
    pthread_t tid[3];

    for (long i = 0; i < 3; i++) {
        if (pthread_create(&tid[i], NULL, thread_func, (void *)(i + 1)) != 0) {
            perror("pthread_create failed");
            exit(EXIT_FAILURE);
        }
        printf("Main: created thread %ld with ID %lu\n", i + 1, (unsigned long)tid[i]);
    }

    for (int i = 0; i < 3; i++) {
        pthread_join(tid[i], NULL);
    }
    return 0;
}

/*
Output:
shrutis@ubuntu:~/SS_HO2/question_58$ gcc question_58.c -o question_58 -pthread
shrutis@ubuntu:~/SS_HO2/question_58$ ./question_58
Main: created thread 1 with ID 139894349362880
Thread 1: my ID (pthread_self) = 139894349362880
Thread 2: my ID (pthread_self) = 139894340970176
Main: created thread 2 with ID 139894340970176
Main: created thread 3 with ID 139894332577472
Thread 3: my ID (pthread_self) = 139894332577472
*/
