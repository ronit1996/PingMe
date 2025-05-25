#include "sys/socket.h"
#include "sys/select.h"
#include "stdlib.h"
#include "stdio.h"
#include "sys/socket.h"
#include "unistd.h"
#include "stdbool.h"
#include "string.h"

void receiveMessage(int *client_socket, fd_set *read_fds_copy){
    if(FD_ISSET(*client_socket, read_fds_copy)){
        // receive server message //
        char message[1056];
        int bytes = recv(*client_socket, &message, sizeof(message)-1, 0);
        message[bytes] = '\0'; // Null termination //
        printf("%s", message);
        fflush(stdout);
    }
}

bool sendMessage(fd_set *read_fds_copy, int *client_socket){
    if(FD_ISSET(STDIN_FILENO, read_fds_copy)){
        char *out_message = NULL;
        size_t len = 0;
        getline(&out_message, &len, stdin);

        if(strcmp(out_message, "EXIT\n") != 0){
            send(*client_socket, out_message, len, 0);
            return true;
        }else{
            free(out_message);
            return false;
        }

        free(out_message);
        return false;
    }

    return true;
}