/* Program Number: 51b (Client)
Student Name: Shruti Sijariya; Register Number: MT2026127
Date: 01-11-2026
Description: Write a program to communicate between two machines using sockets.
              (Client program - run this on machine 2, pass the server's IP)
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define PORT 8080

int main(int argc, char *argv[]) {
    /*
     * CONCEPT: TCP client
     * socket() -> connect() -> write/read -> close()
     * The server's IP address is given on the command line.
     * Find the server machine's IP with: hostname -I
     * inet_pton(): converts the IP text "192.168.x.x" into binary form.
     */
    if (argc != 2) {
        printf("Usage: %s <server_ip>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    int sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (sock_fd == -1) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    if (inet_pton(AF_INET, argv[1], &server_addr.sin_addr) <= 0) {
        printf("Invalid IP address\n");
        exit(EXIT_FAILURE);
    }

    if (connect(sock_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("connect failed");
        exit(EXIT_FAILURE);
    }
    printf("Connected to server %s:%d\n", argv[1], PORT);

    char msg[] = "Hello server, this is the client machine!";
    write(sock_fd, msg, strlen(msg));

    char buffer[1024];
    int n = read(sock_fd, buffer, sizeof(buffer) - 1);
    if (n > 0) {
        buffer[n] = '\0';
        printf("Reply from server: %s\n", buffer);
    }

    close(sock_fd);
    return 0;
}

/*
Output:
shrutis@ubuntu:~/SS_HO2/question_51$ gcc question_51b.c -o question_51b
shrutis@ubuntu:~/SS_HO2/question_51$ ./question_51b 127.0.0.1
Connected to server 127.0.0.1:8080
Reply from server: Hello client, message received by server!
*/
