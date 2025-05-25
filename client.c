#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "stdbool.h"
#include "sys/socket.h"
#include "sys/types.h"
#include "sys/select.h"
#include "arpa/inet.h"
#include "netinet/in.h"
#include "unistd.h"

int main(){
    // create the client socket //
    int client_socket = socket(AF_INET, SOCK_STREAM, 0);

    // fill the struct with IP, Protocol and port //
    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(9002);
    inet_pton(AF_INET, "192.168.29.237", &server_address.sin_addr.s_addr);

    // connect to the server //
    if(connect(client_socket, (const struct sockaddr*)&server_address, sizeof(server_address)) != 0){
        printf("Error, can't connect to the server");
        fflush(stdout);
    }

    // create the fd and fill it with incoming connection and keyboard input //
    fd_set read_fds;
    FD_ZERO(&read_fds);
    FD_SET(client_socket, &read_fds);
    FD_SET(STDIN_FILENO, &read_fds); // keyboard input //
    int max_fd = client_socket > STDIN_FILENO ? client_socket : STDIN_FILENO;

    while(true){
        fd_set read_fds_copy = read_fds;
        select(max_fd+1, &read_fds_copy, NULL, NULL, NULL);

        // check if the current active fd is from client //
        if(FD_ISSET(client_socket, &read_fds_copy)){
            // receive server message //
            char message[1056];
            int bytes = recv(client_socket, &message, sizeof(message)-1, 0);
            message[bytes] = '\0'; // Null termination //
            printf("%s", message);
            fflush(stdout);
        }

        // check if the current active fd is from keyboard //
        if(FD_ISSET(STDIN_FILENO, &read_fds_copy)){
            char *out_message = NULL;
            size_t len = 0;
            getline(&out_message, &len, stdin);

            if(strcmp(out_message, "EXIT\n") != 0){
                send(client_socket, out_message, len, 0);
            }else{
                free(out_message);
                break;
            }

            free(out_message);
        }
    }

    // close the client //
    close(client_socket);
}