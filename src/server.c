#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <signal.h>

#define MAX_CLIENTS 10
#define MAX_NAME_LENGTH 32
#define QUIT_COMMAND "quit"
#define NAME_COMMAND "name"

#define TRUE 1
#define FALSE 0

typedef struct sockaddr SA;


typedef struct client_info {
	int socket;
	char name[MAX_NAME_LENGTH];
	char can_talk;
} client_info;

static client_info *clients[MAX_CLIENTS];

/*
	This is bad because it's not efficient.
	The most efficient way is to assign an ID to each client.
	This way we can map the position in the array to the client.
*/
int8_t register_client(int socket) {
	for (int i = 0; i < MAX_CLIENTS; i++) {
		if (clients[i] == NULL) {
			clients[i] = (client_info*) malloc(sizeof(client_info));
			clients[i]->socket = socket;
			clients[i]->can_talk = FALSE;
			return 0;
		}
	}

	return -1;
}

int8_t unregister_client(int socket) {
	for (int i = 0; i < MAX_CLIENTS; i++) {
		if (clients[i] != NULL && clients[i]->socket == socket) {
			free(clients[i]);
			clients[i] = NULL;
			return 0;
		}
	}

	return -1;
}

int8_t change_client_name(int socket, const char * name) {
	for (int i = 0; i < MAX_CLIENTS; i++) {
		if (clients[i] != NULL && clients[i]->socket == socket) {
			strncpy(clients[i]->name, name, MAX_NAME_LENGTH);
			return 0;
		}
	}

	return -1;
}

int8_t client_can_talk(int socket) {
	for (int i = 0; i < MAX_CLIENTS; i++) {
		if (clients[i] != NULL && clients[i]->socket == socket) {
			return clients[i]->can_talk;
		}
	}

	return -1;
}

int8_t unmute_client(int socket) {
	for (int i = 0; i < MAX_CLIENTS; i++) {
		if (clients[i] != NULL && clients[i]->socket == socket) {
			clients[i]->can_talk = TRUE;
		}
	}

	return -1;
}

void handle_connection(int socket) {
	char buffer[128] = { 0 };
	char name[MAX_NAME_LENGTH] = {0};
	uint8_t quit = 0;

	if (register_client(socket) == -1) {
		fprintf(stderr, "ERROR: unable to register client");
		exit(EXIT_FAILURE);
	}

	while (!quit) {
		memset(buffer, 0, sizeof(buffer));

		read(socket, buffer, sizeof(buffer)); //Check the return and exit the loop if the client disconnected

		if (strncmp(buffer, NAME_COMMAND, strlen(NAME_COMMAND)) == 0) {
			strncpy(name, buffer + strlen(NAME_COMMAND) + 1, MAX_NAME_LENGTH); //name <name>
			change_client_name(socket, name);
			unmute_client(socket);
			
			fprintf(stdout, "Name changed!\n");
		}
		
		if (strncmp(buffer, QUIT_COMMAND, strlen(QUIT_COMMAND)) == 0) {
			quit = 1;
			fprintf(stdout, "Okay bye!\n");
		}

		if (client_can_talk(socket)) { //Check if cient didn't exist
			fprintf(stdout, "Message received!\n%s", buffer); //Print the name like a prompt and signal client to do the same
		}
	}

	fprintf(stdout, "Closing connection!\n");
	close(socket);
	exit(EXIT_FAILURE);
}

int init_server(struct sockaddr_in *server, uint16_t port) {
	int sockfd;

	//Initialize client structure
	for (int i = 0; i < MAX_CLIENTS; i++) {
		clients[i] = NULL;
	}

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
	static struct sockaddr_in server_address, client_address;
	unsigned int client_address_len;
	int port = atoi(argv[1]);
	pid_t pid = 0;

	//Create daemon.
	if ((pid = fork()) == -1) {
		exit(EXIT_FAILURE);
	} else if (pid > 0) {
		exit(EXIT_SUCCESS);
	}
	//Ignore signals.	
	(void)signal(SIGCHLD, SIG_IGN);
	(void)signal(SIGHUP, SIG_IGN);
	

	sockfd = init_server(&server_address, port);
	if (sockfd == EXIT_FAILURE) {
		return EXIT_FAILURE;
	}

	fprintf(stdout, "Waiting for a connection...\n");
	while (1) {
		client_address_len = sizeof(struct sockaddr_in);
		if ((connfd = accept(sockfd, (SA*)&client_address, &client_address_len)) == -1) {
			fprintf(stderr, "ERROR: unable to accept an incoming connection\n");
			return EXIT_FAILURE;
		}

		if ((pid = fork()) == -1) {
			fprintf(stderr, "ERROR: unable to fork\n");
			return EXIT_FAILURE;
		} else if (pid == 0) {
			close(sockfd);
			handle_connection(connfd);
		} else {
			close(connfd);
		}

	}
	
	close(sockfd);
}
