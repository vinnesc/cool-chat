#include <iostream>
#include <cstring>
#include <list>
#include <thread>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <signal.h>

#include "Commands.hpp"
#include "ClientModel.hpp"
#include "ServerController.hpp"
#include "ClientHandler.hpp"

typedef struct sockaddr SA;
 
int init_server(struct sockaddr_in *server, uint16_t port) {
	int sockfd;

	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		std::cerr << "ERROR: unable to create a socket" << std::endl;
		return -1;
	}
	
	server->sin_family = AF_INET;
	server->sin_addr.s_addr = htonl(INADDR_ANY);
	server->sin_port = htons(port);

	if (bind(sockfd, (SA*)server, sizeof(struct sockaddr_in)) == -1) {
		std::cerr << "ERROR: unable to bind the socket" << std::endl;
		return -1;
	}
	
	if (listen(sockfd, 0) == -1) {
		std::cerr << "ERROR: unable to listen from the socket" << std:endl;
		return -1;
	}

	return sockfd;
}

int main(int argc, char **argv) {
	if (argc < 2) {
		std::cerr << "usage: server <port_number>" << std::endl;
		exit(EXIT_FAILURE);
	}

	int sockfd, connfd;
	fd_set m_master;
	static struct sockaddr_in server_address, client_address;
	unsigned int client_address_len;
	int port = atoi(argv[1]);
	bool quit = false;
	auto serverController = std::make_unique<ServerController>();


	sockfd = init_server(&server_address, port);
	if (sockfd == -1) {
		exit(EXIT_FAILURE);
	}

	std::cout << "Waiting for a connection..." << std::endl;
	while (!quit) {
		FD_ZERO(&m_master);

		FD_SET(STDIN_FILENO, &m_master);
		FD_SET(sockfd, &m_master);

		auto retval = select(sockfd + 1, &m_master, NULL, NULL, NULL);
		if (retval == -1) {
			std::cerr << "ERROR: select() failed" << std::endl;
			exit(EXIT_FAILURE);
		}

		if (FD_ISSET(STDIN_FILENO, &m_master)) {
			std::string input;

			std::getline(std::cin, input);
			std::cout << input << std::endl;

			if (input == QUIT_COMMAND) {
				quit = true;
				continue;
			}
		}

		if (FD_ISSET(sockfd, &m_master)) {
			client_address_len = sizeof(struct sockaddr_in);
			if ((connfd = accept(sockfd, (SA*)&client_address, &client_address_len)) == -1) {
				std::cerr << "ERROR: unable to accept an incoming connection" << std::endl;
				exit(EXIT_FAILURE);
			}

			std::cout << "New connection!" << std::endl;
			
			//Create a new client and add it to the clients repository.
			//In theory, I shouldn't have to delete this objects explicitly?
			auto client = std::make_shared<Client>(0, connfd);
			auto clientHandler = std::make_shared<ClientHandler>(client, serverController.get());
			serverController->addClient(client);
			
			std::thread handlerThread(clientHandlerThread, clientHandler);
			handlerThread.detach();
		}
	}
	
	close(sockfd);

	return 0;
}
