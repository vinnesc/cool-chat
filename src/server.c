#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <signal.h>

#include "hash_table.h"
#include "commands.h"

#define MAX_CLIENTS 10
#define MAX_NAME_LENGTH 32

#define TRUE 1
#define FALSE 0

typedef struct sockaddr SA;

typedef struct client_info {
	unsigned int id;

	int socket;
	char name[MAX_NAME_LENGTH];
	char can_talk;
} client_info;

static HashTable *clients;
static unsigned int current_client_id = 0; //This will not be thread safe! :D


//Way better but idk i'm really not thinking about the design so this might be bad still
int8_t register_client(int socket) {
	client_info *new = (client_info*)malloc(sizeof(client_info));
	new->id = current_client_id++;
	new->socket = socket;
	new->can_talk = FALSE;

	insert_hash_table(clients, new->id, new); //Return code

	return new->id;
}

int8_t unregister_client(unsigned int client_id) {
	remove_hash_table(clients, client_id);
}

int8_t change_client_name(unsigned int client_id, const char * name) {
	client_info *ci = (client_info *)get_hash_table(clients, client_id);
	if (ci == NULL) {
		return -1;
	}

	strncpy(ci->name, name, MAX_NAME_LENGTH);

	return 0;
}

int8_t client_can_talk(unsigned int client_id) {
	client_info *ci = (client_info *)get_hash_table(clients, client_id);
	
	if (ci == NULL) {
		return FALSE;
	}

	return ci->can_talk;
}

int8_t unmute_client(unsigned int client_id) {
	client_info *ci = (client_info *)get_hash_table(clients, client_id);
	if (ci == NULL) {
		return -1;
	}

	ci->can_talk = TRUE;

	return 0;
}

void handle_connection(int id, int socket) {
	char buffer[128] = { 0 };
	char name[MAX_NAME_LENGTH] = {0};
	uint8_t quit = 0;

	while (!quit) {
		memset(buffer, 0, sizeof(buffer));

		read(socket, buffer, sizeof(buffer)); //Check the return and exit the loop if the client disconnected

		if (strncmp(buffer, NAME_COMMAND, strlen(NAME_COMMAND)) == 0) {
			strncpy(name, buffer + strlen(NAME_COMMAND) + 1, MAX_NAME_LENGTH); //name <name>
			change_client_name(id, name);
			unmute_client(id);
			
			fprintf(stdout, "Name changed!\n");
		}
		
		if (strncmp(buffer, QUIT_COMMAND, strlen(QUIT_COMMAND)) == 0) {
			quit = 1;
			fprintf(stdout, "Okay bye!\n");
		}

		if (client_can_talk(id)) { //Check if cient didn't exist
			fprintf(stdout, "Message received!\n%s", buffer); //Print the name like a prompt and signal client to do the same
		}
	}

	fprintf(stdout, "Closing connection!\n");
	close(socket);
	exit(EXIT_FAILURE);
}

void free_client_info(void *ci) {
	free((client_info*) ci);
}

int init_server(struct sockaddr_in *server, uint16_t port) {
	int sockfd;

	//Initialize clients structure
	clients = new_hash_table(free_client_info);

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
	unsigned int id;

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

		if ((id = register_client(connfd)) == -1) {
			fprintf(stderr, "ERROR: unable to register client");
			exit(EXIT_FAILURE);
		}

		if ((pid = fork()) == -1) {
			fprintf(stderr, "ERROR: unable to fork\n");
			return EXIT_FAILURE;
		} else if (pid == 0) {
			close(sockfd);
			handle_connection(id, connfd);
		} else {
			close(connfd);
			
			if (unregister_client(connfd) == -1) {
				fprintf(stderr, "ERROR: unable to unregister client");
				exit(EXIT_FAILURE);
			}
		}

	}
	
	close(sockfd);
}
