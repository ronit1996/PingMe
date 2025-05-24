#include "stdio.h"
#include "stdlib.h"
#include "stdbool.h"
#include "string.h"
#include "sys/socket.h"
#include "sys/types.h"
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

    while(true){
        struct sockaddr_in client_address;
        socklen_t addr_len = sizeof(client_address);

        int client = accept(server_socket, (struct sockaddr*)&client_address, &addr_len);
        
        // Get the client IP //
        char client_ip[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &client_address.sin_addr.s_addr, client_ip, INET_ADDRSTRLEN);

        // Check and greet client //
        if(client < 0){
            printf("Error! Couldn't accept client\n");
            fflush(stdout);
            continue;
        }

        // Greet the client //
        printf("-------------------------------\n");
        printf("Hello, %s\n", client_ip);
        fflush(stdout);

        // Send a message to the client //
        char outgoing_message[256] = {0};
        sprintf(outgoing_message, "Hello, %s", client_ip);
        send(client, outgoing_message, strlen(outgoing_message), 0);

        // receive messages from the client and print them //
        while(true){
            char incoming_message[1025] = {0};
            int bytes = recv(client, incoming_message, sizeof(incoming_message)-1, 0);

            // Print messages if it's not empty else release memory , close client and break //
            if(bytes > 0){
                incoming_message[bytes] = '\0'; // Null termination //
                printf("%s: %s\n", client_ip, incoming_message, bytes);
                fflush(stdout);
            }else{
                close(client);
                break;
            }
        }

        // Print the disconnection message //
        printf("Disconnected from %s\n", client_ip);
        fflush(stdout);
    }
}