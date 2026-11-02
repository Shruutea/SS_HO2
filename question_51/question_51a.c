/* Program Number: 51a (Server)
Student Name: Shruti Sijariya; Register Number: MT2026127
Date: 01-11-2026
Description: Write a program to communicate between two machines using sockets.
              (Server program - run this on machine 1)
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define PORT 8080

int main() {
    /*
     * CONCEPT: TCP server (iterative - serves one client)
     * socket() -> bind() -> listen() -> accept() -> read/write -> close()
     * INADDR_ANY: accept connections on every network interface,
     * so a client on ANOTHER machine can connect using this machine's IP.
     * htons(): converts the port to network byte order (big-endian).
     */
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == -1) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    int opt = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, 5) == -1) {
        perror("listen failed");
        exit(EXIT_FAILURE);
    }
    printf("Server waiting for a client on port %d...\n", PORT);

    struct sockaddr_in client_addr;
    socklen_t client_len = sizeof(client_addr);
    int client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &client_len);
    if (client_fd == -1) {
        perror("accept failed");
        exit(EXIT_FAILURE);
    }
    printf("Client connected from %s\n", inet_ntoa(client_addr.sin_addr));

    char buffer[1024];
    int n = read(client_fd, buffer, sizeof(buffer) - 1);
    if (n > 0) {
        buffer[n] = '\0';
        printf("Message from client: %s\n", buffer);
    }

    char reply[] = "Hello client, message received by server!";
    write(client_fd, reply, strlen(reply));

    close(client_fd);
    close(server_fd);
    return 0;
}

/*
Output:
shrutis@ubuntu:~/SS_HO2/question_51$ gcc question_51a.c -o question_51a
shrutis@ubuntu:~/SS_HO2/question_51$ ./question_51a
Server waiting for a client on port 8080...
Client connected from 127.0.0.1
Message from client: Hello server, this is the client machine!
*/
