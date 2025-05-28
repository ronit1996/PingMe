#ifndef MESSAGE_H
#define MESSAGE_H

#define USERNAME_SIZE 32
#define MESSAGE_SIZE 256
#define COLOR_SIZE 7

struct chatMessage{
    char user_username[USERNAME_SIZE];
    char client_username[USERNAME_SIZE];
    char message[MESSAGE_SIZE];
    char commands[MESSAGE_SIZE];
    char color[COLOR_SIZE];
};

void serialize(char *buffer, struct chatMessage *msgStruct);
void deserialize(char *buffer, struct chatMessage *msgStruct);

#endif