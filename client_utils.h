#ifndef CLIENT_UTILS_H
#define CLIENT_UTILS_H
#include "sys/select.h"

void receiveMessage(int *client_socket, fd_set *read_fds_copy);
bool sendMessage(fd_set *read_fds_copy, int *client_socket);

#endif