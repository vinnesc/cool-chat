#include <iostream>
#include <cstring>
#include <unistd.h>

#include "ClientHandler.hpp"
#include "Command.hpp"
#include "commands/NameCommand.hpp"
#include "commands/QuitCommand.hpp"
#include "commands/WhisperCommand.hpp"



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
	std::cout << "Handling...\n";

	while (!quit) {
		char buffer[128] = {0};
		auto error = this->socket->recv(buffer, sizeof(buffer));
		std::cout << "Size in bytes received: " << error << "\n";

		if (error == 0) {
			//The call is blocking so the client must've disconnected
			std::cout << "Client disconnected!\n";
			quit = true;
			continue;
		}

		if (error == -1) {
			std::cerr << "ERROR: Failed to receive from client\n";
			quit = true;
			continue;
		}

		std::string message(buffer);
		
		auto command = Command::deserialize(message);
		if (command->getCommandType() == Commands::NAME) {
			auto name_command = dynamic_cast<NameCommand*>(command);
			name = name_command->getName();

			this->client->changeName(name);
			this->client->unmute();
			
			std::cout << "Name changed!\n";
		} else if (this->client->canTalk()) {
			this->serverController.messageEverybody(message);
		}
		
		if (message == "name") {
			quit = 1;
			std::cout << "Okay bye!\n";
			continue;
		}

		std::cout << "Message received!\n"; //Print the name like a prompt and signal client to do the same
	}

	std::cout << "Closing connection!\n";
	this->serverController.removeClient(client);
	close(this->socket->getSocket());
}

void clientHandlerThread(std::shared_ptr<ClientHandler> handler) {
	handler->handle();
}

