#include "stdio.h"
#include "stdlib.h"
#include "stdbool.h"
#include "string.h"
#include "sys/socket.h"
#include "sys/types.h"
#include "sys/select.h"
#include "arpa/inet.h"
#include "netinet/in.h"
#include "unistd.h"

int main(){
    // creat the server socket //
    int server_socket = socket(AF_INET, SOCK_STREAM, 0);

    // fill the address struct with IP, protocol and port //
    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(9002);
    inet_pton(AF_INET, "0.0.0.0", &server_address.sin_addr.s_addr);

    // bind the address to the server //
    if(bind(server_socket, (struct sockaddr *)&server_address, sizeof(server_address)) != 0){
        printf("Error! Failed to bind the address to the socket\n");
        fflush(stdout);
    }

    // start listening for clients //
    if(listen(server_socket, 5) < 0){
        printf("Error! Failed to listen to clients\n");
        fflush(stdout);
    }

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

        // Loop through all the client sockets in read_fd //
        int fd_recv;
        int client;
        char client_ip[INET_ADDRSTRLEN];

        for(fd_recv = 0; fd_recv <= max_fd; fd_recv++){

            // check if any socket is active using fd_isset() //
            if(FD_ISSET(fd_recv, &read_fd)){

                // check if the socket is active because it's a new connection or a client trying to send message //
                if(fd_recv == server_socket){

                    // if it's a new connection accept it and greet the client also update max_fd //
                    client = accept(server_socket, (struct sockaddr*)&client_address, &addr_len);

                    // Get the client IP //
                    inet_ntop(AF_INET, &client_address.sin_addr.s_addr, client_ip, INET_ADDRSTRLEN);

                    // Check and greet client //
                    if(client < 0){
                        printf("Error! Couldn't accept client\n");
                        fflush(stdout);
                        continue;
                    }

                    // Print client details //
                    printf("-------------------------------\n");
                    printf("Connected to, %s\n", client_ip);
                    fflush(stdout);

                    // Send a message to the client //
                    char outgoing_message[256] = {0};
                    sprintf(outgoing_message, "Hello, %s\n", client_ip);
                    send(client, outgoing_message, strlen(outgoing_message), 0);

                    // increase max fd //
                    max_fd++;

                    // add current client to master fd so that select() can read when the client send messages //
                    FD_SET(client, &master_fd);

                }else{
                    
                    // if a client isn't trying to connect that means it's trying to send a message //
                    // receive messages from the client and print them //
                    char incoming_message[1025] = {0};
                    int bytes = recv(fd_recv, incoming_message, sizeof(incoming_message)-1, 0);

                    // Send the message to all clients if it's not empty
                    if(bytes > 0){
                        // Loop over all the clients store in the read_fd and send to everyone except the server and sender //
                        int fd_send;
                        for(fd_send = 0; fd_send <= max_fd; fd_send++){
                            if(FD_ISSET(fd_send, &master_fd) && fd_send != fd_recv && fd_send != server_socket){
                                printf("check send block\n");
                                fflush(stdout);
                                send(fd_send, incoming_message, strlen(incoming_message), 0);
                            }
                        }
                    }else{
                        close(fd_recv);
                        FD_CLR(fd_recv, &master_fd);
                        // Print the disconnection message //
                        printf("Disconnected from %s\n", client_ip);
                        fflush(stdout);
                    }
                }
            }
        }
        
    }
}