#include <iostream>
#include <cstring>
#include <list>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <signal.h>

#include "commands.hpp"
#include "server.hpp"

#define MAX_CLIENTS 10
#define MAX_NAME_LENGTH 32


typedef struct sockaddr SA;

int send_message(Client c, std::string message) {
	int error;

	error = send(c.get_socket(), message.c_str(), message.length(), 0	);

	return error;
}

void handle_connection(Client c, std::list<Client> clients) {
	char buffer[128];
	std::string name;
	bool quit = false;
	int error;

	while (!quit) {
		error = recv(c.get_socket(), buffer, sizeof(buffer), 0); //Check the return and exit the loop if the client disconnected

		if (error == -1) {
			quit = true;
			continue;
		}

		std::string message(buffer);

		if (message.find(NAME_COMMAND) != std::string::npos) {
			name = message.substr(strlen(NAME_COMMAND) + 1, std::string::npos);
			c.change_name(name);
			c.unmute();
			
			std::cerr << "Name changed!" << std::endl;
		}
		
		if (message == QUIT_COMMAND) {
			quit = 1;
			std::cout << "Okay bye!" << std::endl;
		}

		if (c.can_talk()) { //Check if cient didn't exist
			std::cout << "Message received!" << std::endl; //Print the name like a prompt and signal client to do the same
		}
	}

	std::cout << "Closing connection!" << std::endl;
	close(c.get_socket());
	exit(EXIT_FAILURE);
}	



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
	std::list<Client> clients;

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

		Client c(0, connfd);
		clients.push_back(c);

		if ((pid = fork()) == -1) {
			std::cerr << "ERROR: unable to fork" << std::endl;
			return EXIT_FAILURE;
		} else if (pid == 0) {
			close(sockfd);
			handle_connection(c, clients);
		} else {
			close(connfd);
			clients.remove(c);
		}

	}
	
	close(sockfd);
}
