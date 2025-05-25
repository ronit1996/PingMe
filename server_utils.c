#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/select.h>
#include <string.h>
#include <arpa/inet.h>
#include "server_utils.h"
#include "unistd.h"

void print_client_details(char *client_ip){
    printf("Connected: %s\n", client_ip);
    fflush(stdout);
}

void greet_client(int *client, char *client_ip){
    // Send a message to the client //
    char outgoing_message[256] = {0};
    sprintf(outgoing_message, "Hello, %s\n", client_ip);
    send(*client, outgoing_message, strlen(outgoing_message), 0);
}

void logError(int val, char *message){
    if(val < 0){
        perror(message);
        exit(1);
    }
}

void handle_client(int *server_socket, fd_set *master_fd, int *client, char *client_ip,
    struct sockaddr_in *client_address, socklen_t *addr_len, int *max_fd){
    // if it's a new connection accept it and greet the client also update max_fd //
    *client = accept(*server_socket, (struct sockaddr*)client_address, addr_len);

    // Get the client IP //
    inet_ntop(AF_INET, &(client_address->sin_addr.s_addr), client_ip, INET_ADDRSTRLEN);

    // Check if client got accepted //
    logError(*client, "couldn't accept client");

    // Print client details //
    print_client_details(client_ip);

    // Greet the client //
    greet_client(client, client_ip);

    // increase max fd //
    (*max_fd)++;

    // add current client to master fd so that select() can read when the client send messages //
    FD_SET(*client, master_fd);
}

void sendMessageToAll(int *max_fd, int *fd_recv, int *server_socket, fd_set *master_fd, char *incoming_message){
    // Loop over all the clients store in the read_fd and send to everyone except the server and sender //
    int fd_send;
    for(fd_send = 0; fd_send <= *max_fd; fd_send++){
        if(FD_ISSET(fd_send, master_fd) && fd_send != *fd_recv && fd_send != *server_socket){
            send(fd_send, incoming_message, strlen(incoming_message), 0);
        }
    }
}

void handleDisconnection(int *fd_recv, fd_set *master_fd, char *client_ip){
    close(*fd_recv);
        FD_CLR(*fd_recv, master_fd);
        // Print the disconnection message //
        printf("Disconnected from %s\n", client_ip);
        fflush(stdout);
}

void echoMessage(int *fd_recv, int *max_fd, fd_set *master_fd, int *server_socket, char *client_ip){
    // if a client isn't trying to connect that means it's trying to send a message //
    char incoming_message[1025] = {0};
    int bytes = recv(*fd_recv, incoming_message, sizeof(incoming_message)-1, 0);

    // Send the message to all clients if it's not empty
    if(bytes > 0){
        sendMessageToAll(max_fd, fd_recv, server_socket, master_fd, incoming_message);
    }else{
    // close the client gracefully if cient exits //
    handleDisconnection(fd_recv, master_fd, client_ip);
    }
}