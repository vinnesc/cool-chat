#include <iostream>
#include <cstring>
#include <thread>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>

#include "Commands.hpp"

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

void outboundThread(int sockfd) {
	std::string message;
	bool exit = false;

	while (!exit) {
		std::getline(std::cin, message);

		if (message == QUIT_COMMAND) {
			exit = 1;
			std::cerr << "Okay bye!" << std::endl;
		}
		
		send_message(sockfd, message); //length??
	}
}

void inboundThread(int sockfd) {
	char buffer[128] = {0};
	int retval;

	while (true) {
		if ((retval = recv(sockfd, buffer, sizeof(buffer), 0)) == -1) {

		}
	}
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
	int port = atoi(argv[1]);
	fd_set m_master;
	auto quit = false;

	sockfd = init_client(&server_address, "127.0.0.1", port);

	if (sockfd == EXIT_FAILURE) {
		return EXIT_FAILURE;
	}

	if (connect(sockfd, (SA*)&server_address, sizeof(server_address)) == -1) { 
		std::cerr << "ERROR: unable to connect to the server" << std::endl;
        return EXIT_FAILURE;
    }

	while (!quit) {
		FD_ZERO(&m_master);

		FD_SET(STDIN_FILENO, &m_master);
		FD_SET(sockfd, &m_master);

		auto retval = select(sockfd + 1, &m_master, NULL, NULL, NULL);
		if (retval == -1) {
			std::cerr << "ERROR: select() failed" << std::endl;
			quit = true;
			continue;
		}

		if (FD_ISSET(STDIN_FILENO, &m_master)) {
			std::string input;
			std::getline(std::cin, input);

			if (input == QUIT_COMMAND) {
				quit = true;
				continue;
			} else {
				send_message(sockfd, input);
			}
		}

		if (FD_ISSET(sockfd, &m_master)) {
			char buffer[128] = {0};
			int retval;
			retval = recv(sockfd, buffer, sizeof(buffer), 0);

			if (retval == -1 || retval == 0) {
				std::cerr << "Server probably disconnected" << std::endl;
				quit = true;
				continue;
			}

			std::cout << "Message recieved: " << buffer << std::endl;
		}

	}

	std::cout << "Closing connection!" << std::endl;
	close(sockfd);
	return EXIT_SUCCESS;
}
