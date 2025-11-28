#include <stdio.h>
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

    sock = socket(AF_INET, SOCK_STREAM, 0);

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr));

    // username
    printf("Enter username: ");
    fgets(username, sizeof(username), stdin);
    username[strcspn(username, "\n")] = 0;
    write(sock, username, strlen(username));

    // login response
    bzero(buffer, BUF_SIZE);
    read(sock, buffer, BUF_SIZE);
    printf("%s", buffer);
    fflush(stdout); // MAKE OUTPUT APPEAR IMMEDIATELY


    if (strncmp(buffer, "ERROR", 5) == 0) {
        close(sock);
        return 0;
    }

    // question + "Your answer:"
    bzero(buffer, BUF_SIZE);
    read(sock, buffer, BUF_SIZE);
    printf("%s", buffer);
    fflush(stdout); // MAKE OUTPUT APPEAR IMMEDIATELY


    // answer
    fgets(answer, sizeof(answer), stdin);
    answer[strcspn(answer, "\n")] = 0;
    write(sock, answer, strlen(answer));

    // feedback
    bzero(buffer, BUF_SIZE);
    read(sock, buffer, BUF_SIZE);
    printf("%s", buffer);
    fflush(stdout);

    // active users
    bzero(buffer, BUF_SIZE);
    read(sock, buffer, BUF_SIZE);
    printf("%s", buffer);
    fflush(stdout);

    // end message
    bzero(buffer, BUF_SIZE);
    read(sock, buffer, BUF_SIZE);
    printf("%s", buffer);
    fflush(stdout);

    close(sock);
    return 0;
}
