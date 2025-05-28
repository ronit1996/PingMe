#include <sys/socket.h>
#include <sys/select.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdbool.h>
#include <string.h>
#include "../message.h"

void processArgs(int argc, char *argv[], u_int16_t *port, char *ip){
    if(argc != 3){
        fprintf(stderr, "Usage: %s <port> <ip>", argv[0]);
    }    
    *port = atoi(argv[1]);
    ip = argv[2];
}

void receiveMessage(int *client_socket, char *recvd_data){
    recv(*client_socket, recvd_data, MESSAGE_SIZE, 0);
}

void username(struct chatMessage *msg_details_struct){
    printf("What do you want to be called: ");
    fflush(stdout);

    char *username;
    size_t len = 0;
    getline(&username, &len, stdin);
    // remove the new line character at the end //
    size_t msglen = strlen(username);
    if(len > 0 && username[msglen-1] == '\n'){
        username[msglen-1] = '\0';
    }
    strncpy(msg_details_struct->user_username, username, USERNAME_SIZE);
    free(username);
}

void readInput(struct chatMessage *msg_details){
    char *typed_message;
    size_t len = 0;
    getline(&typed_message, &len, stdin);
    
    // remove the typed line and then display it again properly with username //
    printf("\033[A\33[2K\r");
    printf("%s: %s", msg_details->user_username, typed_message);

    // remove the new line character at the end //
    size_t msglen = strlen(typed_message);
    if(len > 0 && typed_message[msglen-1] == '\n'){
        typed_message[msglen-1] = '\0';
    }

    if(strcmp(typed_message, "EXIT") != 0){
        strncpy(msg_details->message, typed_message, MESSAGE_SIZE);
        free(typed_message);
    }else{
        strncpy(msg_details->commands, typed_message, MESSAGE_SIZE);
        free(typed_message);
    }
}

void printMessage(struct chatMessage *msg_details){
    if(strcmp(msg_details->message, "Null") !=0){
        printf("%s: %s\n", msg_details->client_username, msg_details->message);
        fflush(stdout);
    }
}

bool commandOutput(struct chatMessage *msg_details){
    if(strcmp(msg_details->commands, "EXIT") != 0){
        return true;
    }else{
        return false;
    }
}

void sendMessage(int *client_socket, char *outgoing_data){
    size_t len = MESSAGE_SIZE + USERNAME_SIZE;
    send(*client_socket, outgoing_data, len, 0);
}
