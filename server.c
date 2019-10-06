#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

#define MAX_CLIENTS 10
#define QUIT_COMMAND "quit"

typedef struct sockaddr SA;

static int clients[MAX_CLIENTS];

/*
	This is bad because it's not efficient.
	The most efficient way is to assign an ID to each client.
	This way we can map the position in the array to the client.
*/
int8_t register_client(int socket) {
	for (int i = 0; i < MAX_CLIENTS; i++) {
		if (clients[i] != -1) {
			clients[i] = socket;
			return 0;
		}
	}

	fprintf(stderr, "ERROR: unable to register client\n");
	return 1;
}

int8_t unregister_client(int socket) {
	for (int i = 0; i < MAX_CLIENTS; i++) {
		if (clients[i] == socket) {
			clients[i] = -1;
			return 0;
		}
	}

	fprintf(stderr, "ERROR: unable to uregister client\n");
	return -1;
}

void handle_connection(int socket) {
	char buffer[128] = { 0 };
	uint8_t exit = 0;

	while (!exit) {
		read(socket, buffer, sizeof(buffer));
		fprintf(stdout, "Message received!\n%s\n", buffer);

		if (strncmp(buffer, QUIT_COMMAND, strlen(QUIT_COMMAND)) == 0) {
			exit = 1;
			fprintf(stdout, "Okay bye!\n");
		}
	}
}

int init_server(struct sockaddr_in *server, uint16_t port) {
	int sockfd;

	memset(server, 0, sizeof(struct sockaddr_in));

	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		fprintf(stderr, "ERROR: unable to create a socket\n");
		return EXIT_FAILURE;
	}
	
	server->sin_family = AF_INET;
	server->sin_addr.s_addr = htonl(INADDR_ANY);
	server->sin_port = htons(port);

	if (bind(sockfd, (SA*)server, sizeof(struct sockaddr_in)) == -1) {
		fprintf(stderr, "ERROR: unable to bind the socket\n");
		return EXIT_FAILURE;
	}
	
	if (listen(sockfd, 0) == -1) {
		fprintf(stderr, "ERROR: unable to listen from the socket\n");
		return EXIT_FAILURE;
	}

	return sockfd;
}

int main(int argc, char **argv) {
	if (argc < 2) {
		fprintf(stdout, "usage: server <port_number>\n");
		return EXIT_FAILURE;
	}

	int sockfd, connfd;
	struct sockaddr_in server_address, client_address;
	unsigned int client_address_len = sizeof(struct sockaddr_in);
	int port = atoi(argv[1]);
	pid_t pid = 0;

	sockfd = init_server(&server_address, port);
	
	if (sockfd == EXIT_FAILURE) {
		return EXIT_FAILURE;
	}

	fprintf(stdout, "Waiting for a connection...\n");
	while (1) {
		if ((connfd = accept(sockfd, (SA*)&client_address, &client_address_len)) == -1) {
			fprintf(stderr, "ERROR: unable to accept an incoming connection\n");
			return EXIT_FAILURE;
		}

		if (register_client(connfd)) {
			return EXIT_FAILURE;
		}

		if ((pid = fork() == -1)) {
			fprintf(stderr, "ERROR: unable to fork\n");
			return EXIT_FAILURE;
		} else if (pid == 0) {
			//close(sockfd); This should be closed by the fork and that's it but doesn't work.
			handle_connection(connfd);
		} else {
			close(connfd);
		}

	}
	
	close(sockfd);
}
