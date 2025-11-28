#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUF_SIZE 256

int main() {
    int sock;
    struct sockaddr_in server_addr;
    char buffer[BUF_SIZE];
    char username[50];
    char answer[50];

    // Create socket
    sock = socket(AF_INET, SOCK_STREAM, 0);

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    connect(sock, (struct sockaddr*)&server_addr, sizeof(server_addr));

    // Enter username
    printf("Enter username: ");
    fgets(username, sizeof(username), stdin);
    username[strcspn(username, "\n")] = '\0';

    write(sock, username, strlen(username));

    // Server login reply
    bzero(buffer, BUF_SIZE);
    read(sock, buffer, BUF_SIZE);
    printf("%s", buffer);

    // Failed authentication
    if (strncmp(buffer, "ERROR", 5) == 0) {
        close(sock);
        return 0;
    }

    // Receive question
    bzero(buffer, BUF_SIZE);
    read(sock, buffer, BUF_SIZE);
    printf("%s", buffer);

    // Enter answer
    printf("Your answer: ");
    fgets(answer, sizeof(answer), stdin);
    answer[strcspn(answer, "\n")] = '\0';
    write(sock, answer, strlen(answer));

    // Feedback
    bzero(buffer, BUF_SIZE);
    read(sock, buffer, BUF_SIZE);
    printf("%s", buffer);

    // Active users list
    bzero(buffer, BUF_SIZE);
    read(sock, buffer, BUF_SIZE);
    printf("%s\n", buffer);

    // End message
    bzero(buffer, BUF_SIZE);
    read(sock, buffer, BUF_SIZE);
    printf("%s\n", buffer);

    close(sock);
    return 0;
}
