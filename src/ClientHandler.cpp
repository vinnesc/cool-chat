#include <iostream>
#include <cstring>
#include <unistd.h>

#include "ClientHandler.hpp"
#include "Commands.hpp"

ClientHandler::ClientHandler (
								std::shared_ptr<Client> client,
								std::shared_ptr<SocketBase> socket,
								ServerController &serverController) 
: client(client), socket(socket), serverController(serverController)
{
}

void ClientHandler::handle() {
	std::string name;
	auto quit = false;
	std::cout << "Handling..." << std::endl;

	while (!quit) {
		char buffer[128] = {0};
		auto error = this->socket->recv(buffer, sizeof(buffer));
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
		} else if (this->client->canTalk()) {
			this->serverController.messageEverybody(message);
		}
		
		if (message == QUIT_COMMAND) {
			quit = 1;
			std::cout << "Okay bye!" << std::endl;
			continue;
		}

		std::cout << "Message received!" << std::endl; //Print the name like a prompt and signal client to do the same
	}

	std::cout << "Closing connection!" << std::endl;
	this->serverController.removeClient(client);
	close(this->socket->getSocket());
}

void clientHandlerThread(std::shared_ptr<ClientHandler> handler) {
	handler->handle();
}

