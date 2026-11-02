/* Program Number: 52a (Server)
Student Name: Shruti Sijariya; Register Number: MT2026127
Date: 20-10-2026
Description: Write a program to create a concurrent server:
              a. Use fork
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/wait.h>

#define PORT 8080
#define BUFFER_SIZE 1024

// Reap zombie child processes automatically
void sigchld_handler(int signo) {
    (void)signo;
    while (waitpid(-1, NULL, WNOHANG) > 0);
}

int main() {
    /*
     * CONCEPT: Concurrent TCP Server using fork()
     * 1. socket()  : Creates an IPv4 TCP stream endpoint.
     * 2. bind()    : Associates socket with local port 8080.
     * 3. listen()  : Marks socket as passive listener.
     * 4. accept()  : Blocks until a new client establishes a 3-way handshake.
     * 5. fork()    : Spawns a child process for each incoming client.
     *    - Child  : Closes listening socket, services client requests, then exits.
     *    - Parent : Closes accepted connection socket and continues listening.
     */

    int server_fd, client_fd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_len = sizeof(client_addr);
    char buffer[BUFFER_SIZE];

    // Setup signal handler for child termination to prevent zombies
    struct sigaction sa;
    sa.sa_handler = sigchld_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART | SA_NOCLDSTOP;
    if (sigaction(SIGCHLD, &sa, NULL) == -1) {
        perror("sigaction failed");
        exit(EXIT_FAILURE);
    }

    // Create TCP socket
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == -1) {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Set SO_REUSEADDR to avoid "Address already in use" errors during testing
    int opt = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    // Bind to 0.0.0.0:8080
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("bind failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    // Listen for incoming connections
    if (listen(server_fd, 5) == -1) {
        perror("listen failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    printf("============================================================\n");
    printf("   Concurrent TCP Server running on port %d (PID: %d)       \n", PORT, getpid());
    printf("============================================================\n");

    while (1) {
        client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &client_len);
        if (client_fd < 0) {
            perror("accept failed");
            continue;
        }

        char client_ip[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &client_addr.sin_addr, client_ip, sizeof(client_ip));
        printf("[Main Server PID: %d] Connection accepted from %s:%d\n",
               getpid(), client_ip, ntohs(client_addr.sin_port));

        pid_t pid = fork();

        if (pid < 0) {
            perror("fork failed");
            close(client_fd);
        } else if (pid == 0) {
            // --- CHILD PROCESS ---
            close(server_fd); // Child does not need the master listening socket

            printf("[Worker Child PID: %d] Handling client communication...\n", getpid());
            ssize_t bytes_read = read(client_fd, buffer, sizeof(buffer) - 1);
            if (bytes_read > 0) {
                buffer[bytes_read] = '\0';
                printf("[Worker Child PID: %d] Received: \"%s\"\n", getpid(), buffer);

                // Echo message back with acknowledgment
                const char *ack_prefix = "Server ACK (Processed by Child PID ";
                char reply[BUFFER_SIZE * 2];
                snprintf(reply, sizeof(reply), "%s%d): %s", ack_prefix, getpid(), buffer);
                write(client_fd, reply, strlen(reply));
            }

            close(client_fd);
            printf("[Worker Child PID: %d] Finished servicing client. Exiting.\n", getpid());
            exit(EXIT_SUCCESS);
        } else {
            // --- PARENT PROCESS ---
            close(client_fd); // Parent closes the connected socket and continues to accept()
        }
    }

    close(server_fd);
    return 0;
}


// shrutis@ubuntu:~/SS_HO2/question_52$ ./question_52_server
// ============================================================
//    Concurrent TCP Server running on port 8080 (PID: 9540)       
// ============================================================
// [Main Server PID: 9540] Connection accepted from 127.0.0.1:48210
// [Worker Child PID: 9545] Handling client communication...
// [Worker Child PID: 9545] Received: "Hello from Client 1"
// [Worker Child PID: 9545] Finished servicing client. Exiting

/*
Output:
shrutis@ubuntu:~/SS_HO2/question_52$ gcc question_52a.c -o question_52a
shrutis@ubuntu:~/SS_HO2/question_52$ ./question_52a
============================================================
   Concurrent TCP Server running on port 8080 (PID: 800)       
============================================================
[Main Server PID: 800] Connection accepted from 127.0.0.1:39782
[Main Server PID: 800] Connection accepted from 127.0.0.1:39788
[Worker Child PID: 805] Handling client communication...
[Worker Child PID: 806] Handling client communication...
[Worker Child PID: 806] Received: "Hello from client 2"
[Worker Child PID: 806] Finished servicing client. Exiting.
[Worker Child PID: 805] Received: "Hello from client 1"
[Worker Child PID: 805] Finished servicing client. Exiting.
^C
*/
