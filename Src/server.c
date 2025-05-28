#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/select.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>

#include "server_utils.h"

int main(int argc, char *argv[]){
    // fill in details using the command line arguments //
    u_int16_t port;
    processArgs(argc, argv, &port);

    // creat the server socket //
    int server_socket = socket(AF_INET, SOCK_STREAM, 0);

    // fill the address struct with IP, protocol and port //
    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(port);
    inet_pton(AF_INET, IP, &server_address.sin_addr.s_addr);

    // bind the address to the server //
    logError(bind(server_socket, (struct sockaddr *)&server_address, sizeof(server_address)), "Failed to bind to socket");

    // start listening for clients //
    logError(listen(server_socket, 5), "Failed to listen to client");

    // create the fd set and store the server socket in it //
    fd_set master_fd;
    FD_ZERO(&master_fd);
    FD_SET(server_socket, &master_fd);
    int max_fd = server_socket;

    while(true){

        // create a copy of master_id and fill it with client sockets using select //
        fd_set read_fd = master_fd; 
        select(max_fd+1, &read_fd, NULL, NULL, NULL);

        // create a struct to save client details during connection accept //
        struct sockaddr_in client_address;
        socklen_t addr_len = sizeof(client_address);
        int client;
        char client_ip[INET_ADDRSTRLEN];
        
        // Loop through all the client sockets in read_fd //
        int fd_recv;
        for(fd_recv = 0; fd_recv <= max_fd; fd_recv++){

            // fd_isset() checks if any of the sockets in read_fd match with fd_recv //
            if(FD_ISSET(fd_recv, &read_fd)){

                // if fd_recv matches the server_socket that means the searver_socket is active as a new client wants to connect //
                if(fd_recv == server_socket){

                    handle_client(&server_socket, &master_fd, &client, client_ip, &client_address, &addr_len, &max_fd);

                }else{
                    // Else an existing client is sending a message which the server should echo to all other clients //

                    echoMessage(&fd_recv, &max_fd, &master_fd, &server_socket, client_ip);
                }
            }
        }
        
    }
}