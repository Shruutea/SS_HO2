/* Program Number: 52c (Client)
Student Name: Shruti Sijariya; Register Number: MT2026127
Date: 20-10-2026
Description: TCP client to test the concurrent servers (52a and 52b).
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT 8080
#define SERVER_IP "127.0.0.1"
#define BUFFER_SIZE 1024

int main() {
    int sock_fd;
    struct sockaddr_in serv_addr;
    char message[BUFFER_SIZE];
    char response[BUFFER_SIZE];

    sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (sock_fd == -1) {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    if (inet_pton(AF_INET, SERVER_IP, &serv_addr.sin_addr) <= 0) {
        perror("invalid address / address not supported");
        exit(EXIT_FAILURE);
    }

    if (connect(sock_fd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("connection failed");
        exit(EXIT_FAILURE);
    }

    printf("Connected to server at %s:%d\n", SERVER_IP, PORT);
    printf("Enter message to send: ");
    if (fgets(message, sizeof(message), stdin) != NULL) {
        message[strcspn(message, "\n")] = '\0';
        write(sock_fd, message, strlen(message));
    }

    ssize_t bytes_read = read(sock_fd, response, sizeof(response) - 1);
    if (bytes_read > 0) {
        response[bytes_read] = '\0';
        printf("Response from server: %s\n", response);
    }

    close(sock_fd);
    return 0;
}

// shrutis@ubuntu:~/SS_HO2/question_52$ ./question_52_client
// Connected to server at 127.0.0.1:8080
// Enter message to send: Hello from Client 1
// Response from server: Server ACK (Processed by Child PID 9545): Hello from Client

/*
Output:
shrutis@ubuntu:~/SS_HO2/question_52$ gcc question_52c.c -o question_52c

--- Client 1 (server: question_52a) ---
shrutis@ubuntu:~/SS_HO2/question_52$ ./question_52c
Connected to server at 127.0.0.1:8080
Enter message to send: Hello from client 1
Response from server: Server ACK (Processed by Child PID 805): Hello from client 1

--- Client 2 (server: question_52a) ---
shrutis@ubuntu:~/SS_HO2/question_52$ ./question_52c
Connected to server at 127.0.0.1:8080
Enter message to send: Hello from client 2
Response from server: Server ACK (Processed by Child PID 806): Hello from client 2

--- Client 1 (server: question_52b) ---
shrutis@ubuntu:~/SS_HO2/question_52$ ./question_52c
Connected to server at 127.0.0.1:8080
Enter message to send: Hello from client 1
Response from server: Server ACK (Processed by Thread ID 140150008968896): Hello from client 1

--- Client 2 (server: question_52b) ---
shrutis@ubuntu:~/SS_HO2/question_52$ ./question_52c
Connected to server at 127.0.0.1:8080
Enter message to send: Hello from client 2
Response from server: Server ACK (Processed by Thread ID 140150000576192): Hello from client 2
*/
