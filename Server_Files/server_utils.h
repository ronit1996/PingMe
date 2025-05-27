#ifndef SERVER_UTILS_H
#define SERVER_UTILS_H

#include <sys/select.h>
#include <netinet/in.h>

void handle_client(int *server_socket, fd_set *master_fd, int *client, char *client_ip,
    struct sockaddr_in *client_address, socklen_t *addr_len, int *max_fd);

void logError(int val, char *message);

void echoMessage(int *fd_recv, int *max_fd, fd_set *master_fd, int *server_socket, char *client_ip);

#endif