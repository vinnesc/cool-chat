#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>

#define QUIT_COMMAND "quit"

typedef struct sockaddr SA;

uint8_t send_data(int socket, char *data, size_t length) {    
    ssize_t written_bytes = 0;
	while ((written_bytes += write(socket, data, length - written_bytes)) < length);
    
	printf("Message sent!\n");

    return 0;
}

int init_client(struct sockaddr_in *server, char * address, uint16_t port) {
	int sockfd;

	memset(server, 0, sizeof(struct sockaddr_in));

	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		fprintf(stderr, "ERROR: unable to create a socket");
		return -1;
	}
	
	server->sin_family = AF_INET;
	server->sin_addr.s_addr = inet_addr(address);
	server->sin_port = htons(port);

	return sockfd;
}

int main(int argc, char **argv) {
	if (argc < 2) {
		fprintf(stdout, "usage: server <port_number>\n");
		return EXIT_FAILURE;
	}

	int sockfd;
	struct sockaddr_in server_address;
	int port = atoi(argv[1]);
	uint8_t exit = 0;
	char message_buffer[128] = {0};

	sockfd = init_client(&server_address, "127.0.0.1", port);

	if (sockfd == EXIT_FAILURE) {
		return EXIT_FAILURE;
	}

	if (connect(sockfd, (SA*)&server_address, sizeof(server_address)) == -1) { 
		fprintf(stderr, "ERROR: unable to connect to the server");
        return EXIT_FAILURE;
    }

	while (!exit) {
		fgets(message_buffer, sizeof(message_buffer), stdin); //TODO: check error

		if (strncmp(message_buffer, QUIT_COMMAND, strlen(QUIT_COMMAND)) == 0) {
			exit = 1;
			fprintf(stdout, "Okay bye!\n");
		} else {
			send_data(sockfd, message_buffer, strlen(message_buffer)); //length??
		}
	}


	close(sockfd);
}
