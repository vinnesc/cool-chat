#include <iostream>
#include <cstring>
#include <list>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <signal.h>

#include "Commands.hpp"
#include "ClientModel.hpp"
#include "ServerController.hpp"
#include "ClientHandler.hpp"

#define MAX_CLIENTS 10
#define MAX_NAME_LENGTH 32

typedef struct sockaddr SA;

int init_server(struct sockaddr_in *server, uint16_t port) {
	int sockfd;

	//Initialize clients structure

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
		std::cerr << "usage: server <port_number>" << std::endl;
		return EXIT_FAILURE;
	}

	int sockfd, connfd;
	static struct sockaddr_in server_address, client_address;
	unsigned int client_address_len;
	int port = atoi(argv[1]);
	pid_t pid = 0;
	unsigned int id;

	auto serverController = std::make_unique<ServerController>();
	
	//Create daemon.
	/*
	if ((pid = fork()) == -1) {
		exit(EXIT_FAILURE);
	} else if (pid > 0) {
		exit(EXIT_SUCCESS);
	}
	*/

	//Ignore signals.	
	(void)signal(SIGCHLD, SIG_IGN);
	(void)signal(SIGHUP, SIG_IGN);

	sockfd = init_server(&server_address, port);
	if (sockfd == EXIT_FAILURE) {
		return EXIT_FAILURE;
	}

	std::cout << "Waiting for a connection..." << std::endl;
	while (true) {
		client_address_len = sizeof(struct sockaddr_in);
		if ((connfd = accept(sockfd, (SA*)&client_address, &client_address_len)) == -1) {
			std::cerr << "ERROR: unable to accept an incoming connection" << std::endl;
			return EXIT_FAILURE;
		}

		std::cout << "New connection!" << std::endl;
		
		//Create a new client and add it to the clients repository.
		//In theory, I shouldn't have to delete this objects explicitly?
		auto client = std::make_shared<Client>(0, connfd);
		auto clientHandler = std::make_shared<ClientHandler>(client, serverController.get());
		
		serverController->addClient(client);

		if ((pid = fork()) == -1) {
			std::cerr << "ERROR: unable to fork" << std::endl;
			return EXIT_FAILURE;
		} else if (pid == 0) {
			close(sockfd);
			clientHandler->handle();
		} else {
			close(connfd);
			serverController->removeClient(client);
		}

	}
	
	close(sockfd);
}
