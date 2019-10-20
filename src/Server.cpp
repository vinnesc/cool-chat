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
#include "SocketLinux.hpp"

int init_server(SocketBase &socket) {
	if (socket.bind() == -1) {
		std::cerr << "ERROR: unable to bind the socket" << std::endl;
		return -1;
	}
	
	if (socket.listen() == -1) {
		std::cerr << "ERROR: unable to listen from the socket" << std::endl;
		return -1;
	}

	return 0;
}

int main(int argc, char **argv) {
	if (argc < 2) {
		std::cerr << "usage: server <port_number>" << std::endl;
		exit(EXIT_FAILURE);
	}

	fd_set m_master;
	Port port = atoi(argv[1]);
	auto quit = false;
	auto serverController = std::make_unique<ServerController>();
	auto socket = std::make_unique<SocketLinux>(port);

	if (init_server(*socket) == -1) {
		return EXIT_FAILURE;
	}

	std::cout << "Waiting for a connection..." << std::endl;
	while (!quit) {
		FD_ZERO(&m_master);

		FD_SET(STDIN_FILENO, &m_master);
		FD_SET(socket->getSocket(), &m_master);

		auto retval = select(socket->getSocket() + 1, &m_master, NULL, NULL, NULL);
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
			}
		}

		if (FD_ISSET(socket->getSocket(), &m_master)) {
			auto connfd = socket->accept();
			if (connfd == nullptr) {
				std::cerr << "ERROR: unable to accept an incoming connection" << std::endl;
				quit = true;
				continue;
			}

			std::cout << "New connection!" << std::endl;
			//Create a new client and add it to the clients repository.
			//In theory, I shouldn't have to delete this objects explicitly?
			auto client = std::make_shared<Client>(0);
			auto clientHandler = std::make_shared<ClientHandler>(client, connfd, serverController.get());
			serverController->addClient(client, connfd);
			
			std::thread handlerThread(clientHandlerThread, clientHandler);
			handlerThread.detach();
		}
	}
	
	return 0;
}
