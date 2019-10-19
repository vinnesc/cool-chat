#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <unistd.h>

#include "ClientHandler.hpp"
#include "Commands.hpp"

ClientHandler::ClientHandler(std::shared_ptr<Client> client, ServerController *serverController) {
    this->client = client;
    this->serverController = serverController;
}

void ClientHandler::handle() {
    char buffer[128];
	std::string name;
	auto quit = false;
	std::cout << "Handling..." << std::endl;

	while (!quit) {
		auto error = recv(this->client->getSocket(), buffer, sizeof(buffer), 0);
		std::cout << "Size in bytes received: " << error << std::endl;

		if (error == 0) {
			//The call is blocking so the client must've disconnected
			std::cout << "Client disconnected!" << std::endl;
			quit = true;
			continue;
		}

		if (error == -1) {
			std::cerr << "ERROR: Failed to receive from client" << std::endl;
			quit = true;
			continue;
		}

		std::string message(buffer);

		if (message.find(NAME_COMMAND) != std::string::npos) {
			name = message.substr(strlen(NAME_COMMAND) + 1, std::string::npos);
			this->client->changeName(name);
			this->client->unmute();
			
			std::cout << "Name changed!" << std::endl;
		}
		
		if (message == QUIT_COMMAND) {
			quit = 1;
			std::cout << "Okay bye!" << std::endl;
		}

		if (this->client->canTalk()) {
			std::cout << "Message received!" << std::endl; //Print the name like a prompt and signal client to do the same
			
		}
	}

	std::cout << "Closing connection!" << std::endl;
	this->serverController->removeClient(client);
	close(this->client->getSocket());
}

void clientHandlerThread(std::shared_ptr<ClientHandler> handler) {
	handler->handle();
}

