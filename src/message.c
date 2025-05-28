#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "message.h"

void serialize(char *buffer, struct chatMessage *msgStruct){
    char *userName = msgStruct->user_username;
    char *message = msgStruct->message;
    size_t buff_size = USERNAME_SIZE + MESSAGE_SIZE + 2;
    snprintf(buffer, buff_size, "%s|%s", userName, message);
}

void deserialize(char *buffer, struct chatMessage *msgStruct){
    if(!buffer || !msgStruct) return;
    char *ptr = strtok(buffer, "|");
    strncpy(msgStruct->client_username, ptr ? ptr : "Unknown", USERNAME_SIZE);
    ptr=strtok(NULL, "|");
    strncpy(msgStruct->message, ptr ? ptr : "Null", MESSAGE_SIZE);
}