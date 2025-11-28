#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <arpa/inet.h>

#define PORT 8080
#define MAX_CLIENTS 4
#define BUF_SIZE 256

char *valid_users[] = {"s001", "s002", "s003", "s004", NULL};
char active_users[MAX_CLIENTS][50];
int active_count = 0;

pthread_mutex_t lock;

// ---------------- AUTH ---------------------
int authenticate(char *u) {
    for (int i = 0; valid_users[i] != NULL; i++) {
        if (strcmp(valid_users[i], u) == 0)
            return 1;
    }
    return 0;
}

// -------- ADD USER (thread-safe) ----------
void add_user(char *u) {
    pthread_mutex_lock(&lock);
    strncpy(active_users[active_count], u, 49);
    active_users[active_count][49] = '\0';
    active_count++;
    pthread_mutex_unlock(&lock);
}

// ------- BUILD ACTIVE STUDENT LIST --------
void build_active(char *out) {
    strcpy(out, "Active students: ");
    pthread_mutex_lock(&lock);
    for (int i = 0; i < active_count; i++) {
        strcat(out, active_users[i]);
        if (i < active_count - 1)
            strcat(out, ", ");
    }
    pthread_mutex_unlock(&lock);
}

// ========== HANDLE ONE CLIENT THREAD ==========
void *handle_client(void *arg) {

    int client = *((int *)arg);   // correct unique socket
    char buffer[BUF_SIZE];
    char active_list[256];
    char username_copy[50];

    // ---- Step 1: Receive username ----
    bzero(buffer, BUF_SIZE);
    read(client, buffer, BUF_SIZE);
    buffer[49] = '\0';

    strncpy(username_copy, buffer, 49);
    username_copy[49] = '\0';

    printf("Client attempting login with username: %s\n", buffer);

    // ---- Step 2: Authenticate ----
    if (!authenticate(buffer)) {
        write(client, "ERROR: Authentication failed.\n",
              strlen("ERROR: Authentication failed.\n"));
        close(client);
        pthread_exit(NULL);
    }

    add_user(buffer);

    // ---- Step 3: Welcome ----
    write(client, "Welcome ", strlen("Welcome "));
    write(client, username_copy, strlen(username_copy));
    write(client, "! Authentication successful.\n",
          strlen("! Authentication successful.\n"));

    // ---- Step 4: Send question ----
    const char question[] = "Q: What is 2 + 2?\nYour answer: ";
    write(client, question, strlen(question));

    // ---- Step 5: Receive answer ----
    bzero(buffer, BUF_SIZE);
    read(client, buffer, BUF_SIZE);

    // ---- Step 6: Feedback ----
    if (buffer[0] == '4')
        write(client, "Server: Correct!\n", strlen("Server: Correct!\n"));
    else
        write(client, "Server: Incorrect!\n", strlen("Server: Incorrect!\n"));

    // ---- Step 7: Active users ----
    build_active(active_list);
    write(client, active_list, strlen(active_list));
    write(client, "\n", 1);

    // ---- Step 8: End message ----
    write(client, "Exam session ended. Thank you, ",
          strlen("Exam session ended. Thank you, "));
    write(client, username_copy, strlen(username_copy));
    write(client, "\n", 1);

    close(client);
    pthread_exit(NULL);
}

// =============== MAIN SERVER ====================
int main() {
    int server_fd, client_sock;
    struct sockaddr_in addr;
    int addrlen = sizeof(addr);

    pthread_mutex_init(&lock, NULL);

    server_fd = socket(AF_INET, SOCK_STREAM, 0);

    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(PORT);

    bind(server_fd, (struct sockaddr *)&addr, sizeof(addr));
    listen(server_fd, MAX_CLIENTS);

    printf("Server running on port %d. Waiting for students...\n", PORT);

    while (1) {
        client_sock = accept(server_fd, (struct sockaddr *)&addr,
                             (socklen_t *)&addrlen);
        printf("New student connected.\n");

        int *client_copy = malloc(sizeof(int));
        *client_copy = client_sock;

        pthread_t t;
        pthread_create(&t, NULL, handle_client, client_copy);
        pthread_detach(t);
    }

    close(server_fd);
    return 0;
}
