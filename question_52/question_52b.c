/* Program Number: 52b (Server)
Student Name: Shruti Sijariya; Register Number: MT2026127
Date: 20-10-2026
Description: Write a program to create a concurrent server:
              b. Use pthread_create
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUFFER_SIZE 1024

/*
 * Structure to pass connection details safely into the thread function.
 * Dynamically allocated per client to prevent race conditions on stack descriptors.
 */
typedef struct {
    int client_fd;
    struct sockaddr_in client_addr;
} client_info_t;

/*
 * Worker thread execution routine.
 * Detaching the thread ensures its resources are reclaimed automatically upon exit.
 */
void *client_handler(void *arg) {
    client_info_t *cinfo = (client_info_t *)arg;
    int client_fd = cinfo->client_fd;
    char client_ip[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &(cinfo->client_addr.sin_addr), client_ip, sizeof(client_ip));
    int client_port = ntohs(cinfo->client_addr.sin_port);

    pthread_t tid = pthread_self();
    printf("[Thread ID: %lu] Handling client %s:%d\n", (unsigned long)tid, client_ip, client_port);

    // Free the heap allocated struct for this connection
    free(cinfo);

    char buffer[BUFFER_SIZE];
    ssize_t bytes_read = read(client_fd, buffer, sizeof(buffer) - 1);
    if (bytes_read > 0) {
        buffer[bytes_read] = '\0';
        printf("[Thread ID: %lu] Received message: \"%s\"\n", (unsigned long)tid, buffer);

        // Echo response back to client with Thread ID metadata
        char reply[BUFFER_SIZE * 2];
        snprintf(reply, sizeof(reply), "Server ACK (Processed by Thread ID %lu): %s",
                 (unsigned long)tid, buffer);
        write(client_fd, reply, strlen(reply));
    }

    close(client_fd);
    printf("[Thread ID: %lu] Connection closed and thread terminating.\n", (unsigned long)tid);
    pthread_exit(NULL);
}

int main() {
    /*
     * CONCEPT: Multithreaded TCP Server using POSIX Threads (pthreads)
     * Unlike process-per-client (fork), thread-per-client shares address space,
     * reducing context-switching overhead and memory footprint.
     * 1. socket() / bind() / listen() setup identical to standard TCP servers.
     * 2. accept() blocks waiting for new connections.
     * 3. For every accepted socket, pthread_create() spawns a worker thread.
     * 4. pthread_detach() enables automatic thread resource recycling without pthread_join().
     */

    int server_fd;
    struct sockaddr_in server_addr;

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == -1) {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }

    int opt = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("bind failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, 10) == -1) {
        perror("listen failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    printf("============================================================\n");
    printf("   Multithreaded TCP Server running on port %d (PID: %d)    \n", PORT, getpid());
    printf("============================================================\n");

    while (1) {
        client_info_t *cinfo = (client_info_t *)malloc(sizeof(client_info_t));
        if (!cinfo) {
            perror("malloc failed");
            continue;
        }

        socklen_t client_len = sizeof(cinfo->client_addr);
        cinfo->client_fd = accept(server_fd, (struct sockaddr *)&(cinfo->client_addr), &client_len);
        if (cinfo->client_fd < 0) {
            perror("accept failed");
            free(cinfo);
            continue;
        }

        pthread_t thread_id;
        if (pthread_create(&thread_id, NULL, client_handler, (void *)cinfo) != 0) {
            perror("pthread_create failed");
            close(cinfo->client_fd);
            free(cinfo);
            continue;
        }

        // Detach thread to allow system to release memory upon termination
        pthread_detach(thread_id);
    }

    close(server_fd);
    return 0;
}

// shrutis@ubuntu:~/SS_HO2/question_53$ ./question_53s
// ============================================================
//    Multithreaded TCP Server running on port 8081 (PID: 9812)    
// ============================================================
// [Thread ID: 140234125887232] Handling client 127.0.0.1:49814
// [Thread ID: 140234125887232] Received message: "Hello Multithreaded Server"
// [Thread ID: 140234125887232] Connection closed and thread terminating

/*
Output:
shrutis@ubuntu:~/SS_HO2/question_52$ gcc question_52b.c -o question_52b -pthread
shrutis@ubuntu:~/SS_HO2/question_52$ ./question_52b
============================================================
   Multithreaded TCP Server running on port 8080 (PID: 809)    
============================================================
[Thread ID: 140150008968896] Handling client 127.0.0.1:39798
[Thread ID: 140150008968896] Received message: "Hello from client 1"
[Thread ID: 140150000576192] Handling client 127.0.0.1:39806
[Thread ID: 140150000576192] Received message: "Hello from client 2"
[Thread ID: 140150000576192] Connection closed and thread terminating.
[Thread ID: 140150008968896] Connection closed and thread terminating.
^C
*/
