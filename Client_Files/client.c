#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/select.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>

#include "client_utils.h"
#include "../message.h"

int main(int argc, char *argv[]){
    // process the command line arguments //
    u_int16_t port;
    char *ip;
    processArgs(argc, argv, &port, ip);
    
    // create the message details struct //
    struct chatMessage message_details;

    // ask for username //
    username(&message_details);
    
    // create the client socket //
    int client_socket = socket(AF_INET, SOCK_STREAM, 0);

    // fill the struct with IP, Protocol and port //
    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(port);
    inet_pton(AF_INET, ip, &server_address.sin_addr.s_addr);

    // connect to the server //
    if(connect(client_socket, (const struct sockaddr*)&server_address, sizeof(server_address)) != 0){
        printf("Error, can't connect to the server");
        fflush(stdout);
    }

    // create the fd and fill it with incoming connection and keyboard input //
    fd_set socket_fds;
    FD_ZERO(&socket_fds);
    FD_SET(client_socket, &socket_fds);
    FD_SET(STDIN_FILENO, &socket_fds); // keyboard input //
    int max_fd = client_socket > STDIN_FILENO ? client_socket : STDIN_FILENO;
    bool check = true;
    
    while(check){
        // clearn the struct data from previous loop //
        memset(&message_details.commands, 0, sizeof(message_details.commands));
        memset(&message_details.message, 0, sizeof(message_details.message));
        memset(&message_details.client_username, 0, sizeof(message_details.client_username));

        fd_set temp_fds = socket_fds;
        select(max_fd+1, &temp_fds, NULL, NULL, NULL);

        if(FD_ISSET(client_socket, &temp_fds)){
            // recieve the incoming data from the server //
            char incoming_data[MESSAGE_SIZE + USERNAME_SIZE + USERNAME_SIZE] = {0};
            receiveMessage(&client_socket, incoming_data);

            // deserialize the message //
            deserialize(incoming_data, &message_details);

            // print the message //
            printMessage(&message_details);
        }

        if(FD_ISSET(STDIN_FILENO, &temp_fds)){
            // read input of the user //
            readInput(&message_details);

            // output code for commands //
            check = commandOutput(&message_details);

            // serialize the input //
            char outgoing_data[MESSAGE_SIZE + USERNAME_SIZE + USERNAME_SIZE] = {0};
            serialize(outgoing_data, &message_details);

            // check if the current active fd is from keyboard //
            sendMessage(&client_socket, outgoing_data);
        }
    }

    // close the client //
    close(client_socket);
}