#include <iostream>
#include <cstring>
#include <list>
#include <thread>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <signal.h>

#include "ClientModel.hpp"
#include "ServerController.hpp"
#include "ClientHandler.hpp"
#include "SocketLinux.hpp"

int init_server(SocketLinux &socket) {
	if (socket.bind() == -1) {
		perror("ERROR: bind() failed");
		return -1;
	}
	
	if (socket.listen() == -1) {
		perror("ERROR: listen() failed");
		return -1;
	}

	return 0;
}

int main(int argc, char **argv) {
	if (argc < 2) {
		std::cerr << "usage: server <port_number>\n";
		exit(EXIT_FAILURE);
	}

	fd_set m_master;
	Port port = atoi(argv[1]);
	auto quit = false;

	//Is this the way it should be done?
	SocketLinux socket(port);
	ServerController serverController;
	ClientHandler handler(serverController, socket);

	if (init_server(dynamic_cast<SocketLinux&>(socket)) == -1) {
		return EXIT_FAILURE;
	}

	std::cout << "Waiting for a connection...\n";
	std::thread handlerThread(clientHandlerThread, std::ref(handler));
	handlerThread.detach();
	while (!quit) {
		std::string input;
		std::getline(std::cin, input);
		std::cout << input;

		if (input == "quit") {
			quit = true;
			continue;
		}
	}

	return 0;
}
