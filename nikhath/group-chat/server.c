#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <fcntl.h>
#include <errno.h>

#define MAX_CLIENTS 100
#define MAX_EVENTS 100
#define BUFFER_SIZE 2048
#define NAME_LEN 32

typedef struct {
    struct sockaddr_in address;
    int sockfd;
    int uid;
    char name[NAME_LEN];
} client_t;

typedef struct {
    char message[BUFFER_SIZE + NAME_LEN];
    pthread_t thread_id;
} broadcast_job_t;

client_t *clients[MAX_CLIENTS];
pthread_mutex_t clients_mutex = PTHREAD_MUTEX_INITIALIZER;

int client_count = 0;
int uid_counter = 0;

void str_trim_lf(char* arr, int length) {
    for (int i = 0; i < length; i++) {
        if (arr[i] == '\n') {
            arr[i] = '\0';
            break;
        }
    }
}

int set_nonblocking(int sockfd) {
    int flags = fcntl(sockfd, F_GETFL, 0);
    if (flags == -1) return -1;
    return fcntl(sockfd, F_SETFL, flags | O_NONBLOCK);
}

void add_client(client_t *cl) {
    pthread_mutex_lock(&clients_mutex);
    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (!clients[i]) {
            clients[i] = cl;
            client_count++;
            break;
        }
    }
    pthread_mutex_unlock(&clients_mutex);
}

void remove_client(int uid) {
    pthread_mutex_lock(&clients_mutex);
    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (clients[i] && clients[i]->uid == uid) {
            clients[i] = NULL;
            client_count--;
            break;
        }
    }
    pthread_mutex_unlock(&clients_mutex);
}

client_t* get_client_by_sockfd(int sockfd) {
    client_t *result = NULL;
    pthread_mutex_lock(&clients_mutex);
    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (clients[i] && clients[i]->sockfd == sockfd) {
            result = clients[i];
            break;
        }
    }
    pthread_mutex_unlock(&clients_mutex);
    return result;
}

void *broadcast_thread(void *arg) {
    broadcast_job_t *job = (broadcast_job_t *)arg;

    pthread_mutex_lock(&clients_mutex);
    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (clients[i]) {
            write(clients[i]->sockfd, job->message, strlen(job->message));
        }
    }
    pthread_mutex_unlock(&clients_mutex);

    free(job);
    pthread_detach(pthread_self());
    return NULL;
}

void *broadcast_except_thread(void *arg) {
    broadcast_job_t *job = (broadcast_job_t *)arg;
    int sender_uid = *((int *)&job->message[BUFFER_SIZE + NAME_LEN - sizeof(int)]);

    pthread_mutex_lock(&clients_mutex);
    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (clients[i] && clients[i]->uid != sender_uid) {
            write(clients[i]->sockfd, job->message, strlen(job->message));
        }
    }
    pthread_mutex_unlock(&clients_mutex);

    free(job);
    pthread_detach(pthread_self());
    return NULL;
}

void send_message_all(char *s) {
    broadcast_job_t *job = malloc(sizeof(broadcast_job_t));
    strncpy(job->message, s, BUFFER_SIZE + NAME_LEN - 1);
    pthread_create(&job->thread_id, NULL, &broadcast_thread, (void*)job);
}

void send_message_except(char *s, int uid) {
    broadcast_job_t *job = malloc(sizeof(broadcast_job_t));
    strncpy(job->message, s, BUFFER_SIZE + NAME_LEN - 1);
    memcpy(&job->message[BUFFER_SIZE + NAME_LEN - sizeof(int)], &uid, sizeof(int));
    pthread_create(&job->thread_id, NULL, &broadcast_except_thread, (void*)job);
}

void send_active_clients(int sockfd) {
    char list[BUFFER_SIZE];
    memset(list, 0, BUFFER_SIZE);

    pthread_mutex_lock(&clients_mutex);
    sprintf(list, "<<< Active users: ");
    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (clients[i]) {
            strcat(list, clients[i]->name);
            strcat(list, ", ");
        }
    }
    pthread_mutex_unlock(&clients_mutex);

    int len = strlen(list);
    if (len > 2) {
        list[len-2] = '\0';
    }
    strcat(list, " >>>\n");
    write(sockfd, list, strlen(list));
}

void handle_client_message(int sockfd, char *buffer) {
    client_t *cli = get_client_by_sockfd(sockfd);
    if (!cli) return;

    char message[BUFFER_SIZE + NAME_LEN];
    str_trim_lf(buffer, strlen(buffer));

    if (strcmp(buffer, "/users") == 0) {
        send_active_clients(cli->sockfd);
    } else if (strlen(buffer) > 0) {
        snprintf(message, sizeof(message), "[%s] > %s\n", cli->name, buffer);
        printf("%s", message);
        send_message_except(message, cli->uid);
    }
}

void handle_client_disconnect(int epollfd, int sockfd) {
    client_t *cli = get_client_by_sockfd(sockfd);
    if (cli) {
        char message[BUFFER_SIZE];
        sprintf(message, "<<< %s has left the chat >>>\n", cli->name);
        printf("%s", message);
        send_message_all(message);

        epoll_ctl(epollfd, EPOLL_CTL_DEL, sockfd, NULL);
        close(sockfd);
        remove_client(cli->uid);
        free(cli);
    }
}

int main(int argc, char **argv) {
    if (argc != 2) {
        printf("Usage: %s <port>\n", argv[0]);
        return EXIT_FAILURE;
    }

    char *ip = "0.0.0.0";
    int port = atoi(argv[1]);
    int option = 1;
    int listenfd, connfd, epollfd;
    struct sockaddr_in serv_addr, cli_addr;
    struct epoll_event ev, events[MAX_EVENTS];

    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    if (listenfd < 0) {
        perror("ERROR: Socket failed");
        return EXIT_FAILURE;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(ip);
    serv_addr.sin_port = htons(port);

    if (setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option)) < 0) {
        perror("ERROR: setsockopt failed");
        return EXIT_FAILURE;
    }

    if (bind(listenfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("ERROR: Bind failed");
        return EXIT_FAILURE;
    }

    if (listen(listenfd, 10) < 0) {
        perror("ERROR: Listen failed");
        return EXIT_FAILURE;
    }

    set_nonblocking(listenfd);

    epollfd = epoll_create1(0);
    if (epollfd < 0) {
        perror("ERROR: epoll_create1 failed");
        return EXIT_FAILURE;
    }

    ev.events = EPOLLIN;
    ev.data.fd = listenfd;
    if (epoll_ctl(epollfd, EPOLL_CTL_ADD, listenfd, &ev) < 0) {
        perror("ERROR: epoll_ctl failed");
        return EXIT_FAILURE;
    }

    printf("=== GROUP CHAT SERVER (EPOLL + MULTI-THREADING) ===\n");
    printf("Server started on %s:%d\n", ip, port);
    printf("Waiting for connections...\n");

    while (1) {
        int nfds = epoll_wait(epollfd, events, MAX_EVENTS, -1);
        if (nfds < 0) {
            perror("ERROR: epoll_wait failed");
            continue;
        }

        for (int i = 0; i < nfds; i++) {
            if (events[i].data.fd == listenfd) {
                socklen_t clilen = sizeof(cli_addr);
                connfd = accept(listenfd, (struct sockaddr*)&cli_addr, &clilen);

                if (connfd < 0) {
                    if (errno != EAGAIN && errno != EWOULDBLOCK) {
                        perror("ERROR: Accept failed");
                    }
                    continue;
                }

                if (client_count >= MAX_CLIENTS) {
                    printf("Max clients reached. Connection rejected.\n");
                    close(connfd);
                    continue;
                }

                client_t *cli = (client_t *)malloc(sizeof(client_t));
                cli->address = cli_addr;
                cli->sockfd = connfd;
                cli->uid = uid_counter++;

                if (recv(connfd, cli->name, NAME_LEN, 0) <= 0) {
                    printf("Failed to receive client name\n");
                    close(connfd);
                    free(cli);
                    continue;
                }
                str_trim_lf(cli->name, strlen(cli->name));

                if (strlen(cli->name) < 2 || strlen(cli->name) >= NAME_LEN - 1) {
                    printf("Invalid name length\n");
                    close(connfd);
                    free(cli);
                    continue;
                }

                set_nonblocking(connfd);

                ev.events = EPOLLIN | EPOLLET;
                ev.data.fd = connfd;
                if (epoll_ctl(epollfd, EPOLL_CTL_ADD, connfd, &ev) < 0) {
                    perror("ERROR: epoll_ctl add client failed");
                    close(connfd);
                    free(cli);
                    continue;
                }

                printf("Client %s connected from %s:%d\n", cli->name,
                       inet_ntoa(cli_addr.sin_addr), ntohs(cli_addr.sin_port));

                add_client(cli);

                char message[BUFFER_SIZE];
                sprintf(message, "<<< %s has joined the chat >>>\n", cli->name);
                printf("%s", message);
                send_message_all(message);
                send_active_clients(cli->sockfd);

            } else {
                char buffer[BUFFER_SIZE];
                memset(buffer, 0, BUFFER_SIZE);

                int sockfd = events[i].data.fd;
                int receive = recv(sockfd, buffer, BUFFER_SIZE, 0);

                if (receive > 0) {
                    handle_client_message(sockfd, buffer);
                } else if (receive == 0) {
                    handle_client_disconnect(epollfd, sockfd);
                } else {
                    if (errno != EAGAIN && errno != EWOULDBLOCK) {
                        handle_client_disconnect(epollfd, sockfd);
                    }
                }
            }
        }
    }

    close(epollfd);
    close(listenfd);
    return EXIT_SUCCESS;
}
