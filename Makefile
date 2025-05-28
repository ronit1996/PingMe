# Compiler
CC = gcc

# Compiler flags
CFLAGS = -Wall -Wextra

# Server source files
SERVER_SRCS = src/server.c src/server_utils.c src/message.c
SERVER_BIN = server

# Client source files
CLIENT_SRCS = src/client.c src/client_utils.c src/message.c
CLIENT_BIN = client

# Build both by default
all: $(SERVER_BIN) $(CLIENT_BIN)

# Build server
$(SERVER_BIN): $(SERVER_SRCS)
	$(CC) $(CFLAGS) $(SERVER_SRCS) -o $(SERVER_BIN)

# Build client with readline
$(CLIENT_BIN): $(CLIENT_SRCS)
	$(CC) $(CFLAGS) $(CLIENT_SRCS) -o $(CLIENT_BIN)

# Clean build artifacts
clean:
	rm -f $(SERVER_BIN) $(CLIENT_BIN)
