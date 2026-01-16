#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <pthread.h>
#include <signal.h>

#define BUFFER_SIZE 2048
#define NAME_LEN 32

volatile sig_atomic_t flag = 0;
int sockfd = 0;
char name[NAME_LEN];

void str_overwrite_stdout() {
    printf("[%s] > ", name);
    fflush(stdout);
}

void str_trim_lf(char* arr, int length) {
    for (int i = 0; i < length; i++) {
        if (arr[i] == '\n') {
            arr[i] = '\0';
            break;
        }
    }
}

void catch_ctrl_c(int sig) {
    flag = 1;
}

void send_msg_handler() {
    char message[BUFFER_SIZE] = {};
    char buffer[BUFFER_SIZE + NAME_LEN + 10] = {};

    while (1) {
        str_overwrite_stdout();
        fgets(message, BUFFER_SIZE, stdin);
        str_trim_lf(message, BUFFER_SIZE);

        if (strcmp(message, "/quit") == 0) {
            break;
        } else if (strcmp(message, "/users") == 0) {
            send(sockfd, message, strlen(message), 0);
        } else {
            if (strlen(message) > 0) {
                send(sockfd, message, strlen(message), 0);
            }
        }

        memset(message, 0, BUFFER_SIZE);
        memset(buffer, 0, BUFFER_SIZE + NAME_LEN + 10);
    }
    catch_ctrl_c(2);
}

void recv_msg_handler() {
    char message[BUFFER_SIZE] = {};
    while (1) {
        int receive = recv(sockfd, message, BUFFER_SIZE, 0);
        if (receive > 0) {
            printf("\r\033[K%s", message);
            str_overwrite_stdout();
        } else if (receive == 0) {
            break;
        }
        memset(message, 0, sizeof(message));
    }
}

int main(int argc, char **argv) {
    if (argc != 3) {
        printf("Usage: %s <IP> <port>\n", argv[0]);
        return EXIT_FAILURE;
    }

    char *ip = argv[1];
    int port = atoi(argv[2]);

    signal(SIGINT, catch_ctrl_c);

    printf("Enter your name: ");
    fgets(name, NAME_LEN, stdin);
    str_trim_lf(name, strlen(name));

    if (strlen(name) > NAME_LEN - 1 || strlen(name) < 2) {
        printf("Name must be between 2 and %d characters.\n", NAME_LEN - 1);
        return EXIT_FAILURE;
    }

    struct addrinfo hints, *servinfo, *p;
    int rv;
    char port_str[6];

    snprintf(port_str, sizeof(port_str), "%d", port);

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;

    if ((rv = getaddrinfo(ip, port_str, &hints, &servinfo)) != 0) {
        printf("ERROR: getaddrinfo: %s\n", gai_strerror(rv));
        return EXIT_FAILURE;
    }

    for (p = servinfo; p != NULL; p = p->ai_next) {
        if ((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
            perror("ERROR: socket");
            continue;
        }

        if (connect(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
            close(sockfd);
            perror("ERROR: connect");
            continue;
        }

        break;
    }

    if (p == NULL) {
        printf("ERROR: Failed to connect to server %s:%d\n", ip, port);
        freeaddrinfo(servinfo);
        return EXIT_FAILURE;
    }

    freeaddrinfo(servinfo);

    send(sockfd, name, NAME_LEN, 0);

    printf("=== WELCOME TO THE GROUP CHAT ===\n");
    printf("Type /users to see active users\n");
    printf("Type /quit to exit\n");

    pthread_t send_msg_thread;
    if (pthread_create(&send_msg_thread, NULL, (void *) send_msg_handler, NULL) != 0) {
        printf("ERROR: pthread failed\n");
        return EXIT_FAILURE;
    }

    pthread_t recv_msg_thread;
    if (pthread_create(&recv_msg_thread, NULL, (void *) recv_msg_handler, NULL) != 0) {
        printf("ERROR: pthread failed\n");
        return EXIT_FAILURE;
    }

    while (1) {
        if (flag) {
            printf("\nGoodbye!\n");
            break;
        }
    }

    close(sockfd);

    return EXIT_SUCCESS;
}
