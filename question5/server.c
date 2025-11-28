#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <arpa/inet.h>

#define PORT 8080
#define MAX_CLIENTS 4
#define BUF_SIZE 256

// ================================================================
// Hard-coded valid usernames
// ================================================================
char* valid_users[] = {
    "s001", "s002", "s003", "s004",
    NULL
};

// Track authenticated students
char active_users[MAX_CLIENTS][50];
int active_count = 0;

pthread_mutex_t lock;

// ================================================================
// Check if a username is valid
// ================================================================
int authenticate(char* username) {
    for (int i = 0; valid_users[i] != NULL; i++) {
        if (strcmp(username, valid_users[i]) == 0)
            return 1;
    }
    return 0;
}

// ================================================================
// Add a student to the active users list
// ================================================================
void add_user(char* username) {
    pthread_mutex_lock(&lock);
    strncpy(active_users[active_count], username, 49);
    active_users[active_count][49] = '\0';
    active_count++;
    pthread_mutex_unlock(&lock);
}

// ================================================================
// Build a string containing active students
// ================================================================
void get_active_users(char* out) {
    strcpy(out, "Active students: ");

    pthread_mutex_lock(&lock);
    for (int i = 0; i < active_count; i++) {
        strcat(out, active_users[i]);
        if (i < active_count - 1)
            strcat(out, ", ");
    }
    pthread_mutex_unlock(&lock);
}

// ================================================================
// Thread function that handles a single client
// ================================================================
void* handle_client(void* arg) {
    int client_sock = *(int*)arg;
    char buffer[BUF_SIZE];
    char login_msg[256];
    char end_msg[256];
    char active_msg[256];

    // ------------------------------------------------------------
    // Step 1: Receive username
    // ------------------------------------------------------------
    bzero(buffer, BUF_SIZE);
    read(client_sock, buffer, BUF_SIZE);

    // Limit username to 49 chars (prevents warnings & overflow)
    buffer[49] = '\0';

    printf("Client attempting login with username: %s\n", buffer);

    // ------------------------------------------------------------
    // Step 2: Authenticate student
    // ------------------------------------------------------------
    if (!authenticate(buffer)) {
        char* msg = "ERROR: Authentication failed.\n";
        write(client_sock, msg, strlen(msg));
        close(client_sock);
        pthread_exit(NULL);
    }

    // Add to active user list
    add_user(buffer);

    // ------------------------------------------------------------
    // Step 3: Send welcome message (safe printing)
    // ------------------------------------------------------------
    snprintf(login_msg, sizeof(login_msg),
             "Welcome %.49s! Authentication successful.\n", buffer);
    write(client_sock, login_msg, strlen(login_msg));

    // ------------------------------------------------------------
    // Step 4: Send exam question
    // ------------------------------------------------------------
    char* question = "Q: What is 2 + 2? ";
    write(client_sock, question, strlen(question));

    // ------------------------------------------------------------
    // Step 5: Read answer from student
    // ------------------------------------------------------------
    bzero(buffer, BUF_SIZE);
    read(client_sock, buffer, BUF_SIZE);

    // ------------------------------------------------------------
    // Step 6: Check answer accuracy
    // ------------------------------------------------------------
    if (strcmp(buffer, "4") == 0 || strcmp(buffer, "4\n") == 0) {
        write(client_sock, "Server: Correct!\n", 18);
    } else {
        write(client_sock, "Server: Incorrect.\n", 20);
    }

    // ------------------------------------------------------------
    // Step 7: Send active user list
    // ------------------------------------------------------------
    get_active_users(active_msg);
    write(client_sock, active_msg, strlen(active_msg));

    // ------------------------------------------------------------
    // Step 8: End exam session
    // ------------------------------------------------------------
    snprintf(end_msg, sizeof(end_msg),
             "Exam session ended. Thank you, %.49s\n", buffer);
    write(client_sock, end_msg, strlen(end_msg));

    close(client_sock);
    pthread_exit(NULL);
}

// ================================================================
// Main server function
// ================================================================
int main() {
    int server_fd, client_sock;
    struct sockaddr_in address;
    int addrlen = sizeof(address);

    pthread_mutex_init(&lock, NULL);

    // Create TCP socket
    server_fd = socket(AF_INET, SOCK_STREAM, 0);

    // Server address settings
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // Bind & listen
    bind(server_fd, (struct sockaddr*)&address, sizeof(address));
    listen(server_fd, MAX_CLIENTS);

    printf("Server running on port %d. Waiting for students...\n", PORT);

    // Accept up to MAX_CLIENTS concurrently
    while (1) {
        client_sock = accept(server_fd, (struct sockaddr*)&address, (socklen_t*)&addrlen);
        printf("New student connected.\n");

        pthread_t thread;
        pthread_create(&thread, NULL, handle_client, &client_sock);
        pthread_detach(thread);
    }

    close(server_fd);
    return 0;
}
