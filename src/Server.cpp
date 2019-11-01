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

int init_server(SocketBase &socket) {
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
	ClientHandler handler(serverController);

	if (init_server(dynamic_cast<SocketBase&>(socket)) == -1) {
		return EXIT_FAILURE;
	}

	std::cout << "Waiting for a connection...\n";
	std::thread handlerThread(clientHandlerThread, std::ref(handler));
	while (!quit) {
		FD_ZERO(&m_master);

		FD_SET(STDIN_FILENO, &m_master);
		FD_SET(socket.getSocket(), &m_master);

		auto retval = select(socket.getSocket() + 1, &m_master, NULL, NULL, NULL);
		if (retval == -1) {
			perror("ERROR: select() failed");
			quit = true;
			continue;
		}

		if (FD_ISSET(STDIN_FILENO, &m_master)) {
			std::string input;
			std::getline(std::cin, input);

			if (input == "quit") {
				quit = true;
				continue;
			}
		}

		if (FD_ISSET(socket.getSocket(), &m_master)) {
			auto connection_socket = socket.accept();
			if (connection_socket == nullptr) {
				std::cerr << "ERROR: unable to accept an incoming connection\n";
				quit = true;
				continue;
			}

			std::cout << "New connection!\n";
			//Create a new client and add it to the clients repository.
			//In theory, I shouldn't have to delete this objects explicitly?
			auto client = std::make_shared<Client>(0); 	//TO-DO: Implement clients ID
			serverController.addClient(client, connection_socket);
		}
	}
	handlerThread.join();

	return 0;
}
