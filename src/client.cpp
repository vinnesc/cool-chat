#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>

#include "commands.hpp"

typedef struct sockaddr SA;

int send_message(int socket, std::string message) {    
    std::size_t written_bytes = 0;
	std::size_t error = 0;
	std::size_t length = message.length();

	while ((error = write(socket, message.c_str(), length - written_bytes)) < length) {
		if (error == -1) {
			return error;
		}

		written_bytes += error;
	}
    
	std::cout << "Message sent" << std::endl;

    return 0;
}

int init_client(struct sockaddr_in *server, std::string address, int port) {
	int sockfd;

	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		std::cerr << "ERROR: unable to create a socket" << std::endl;
		return -1;
	}
	
	server->sin_family = AF_INET;
	server->sin_addr.s_addr = inet_addr(address.c_str());
	server->sin_port = htons(port);

	return sockfd;
}

int main(int argc, char **argv) {
	if (argc < 2) {
		std::cerr << "usage: server <port_number>" << std::endl;
		return EXIT_FAILURE;
	}

	int sockfd;
	static struct sockaddr_in server_address;
	std::string message;
	int port = atoi(argv[1]);
	bool exit = false;

	sockfd = init_client(&server_address, "127.0.0.1", port);

	if (sockfd == EXIT_FAILURE) {
		return EXIT_FAILURE;
	}

	if (connect(sockfd, (SA*)&server_address, sizeof(server_address)) == -1) { 
		std::cerr << "ERROR: unable to connect to the server" << std::endl;
        return EXIT_FAILURE;
    }

	while (!exit) {
		std::getline(std::cin, message);

		if (message == QUIT_COMMAND) {
			exit = 1;
			std::cerr << "Okay bye!" << std::endl;
		}
		
		send_message(sockfd, message); //length??
	}

	std::cout << "Closing connection!" << std::endl;

	return EXIT_SUCCESS;
}
