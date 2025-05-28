#ifndef CLIENT_UTILS_H
#define CLIENT_UTILS_H
#include "sys/select.h"
#include "message.h"

void processArgs(int argc, char *argv[], u_int16_t *port, char **ip);
void receiveMessage(int *client_socket, char *recvd_data);
void sendMessage(int *client_socket, char *outgoing_data);
void username(struct chatMessage *msg_details_struct);
void printMessage(struct chatMessage *msg_details);
void readInput(struct chatMessage *msg_details);
bool commandOutput(struct chatMessage *msg_details);
void logError(int *return_val, int check_val, char *message);

#endif