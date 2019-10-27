#include <iostream>
#include <cstring>
#include <unistd.h>

#include "ClientHandler.hpp"
#include "Command.hpp"
#include "commands/NameCommand.hpp"
#include "commands/QuitCommand.hpp"
#include "commands/WhisperCommand.hpp"


ClientHandler::ClientHandler (ServerController& serverController) 
: serverController(serverController)
{
}

Socket ClientHandler::registerFileDescriptors(fd_set& master) {
	Socket max = -1;
	for (auto& client : this->serverController.getClientsSockets()) {
		auto socket = client.second->getSocket();
		FD_SET(socket, &master);
		
		if (socket > max) {
			max = socket;
		}
	}

	return max;
}

void ClientHandler::handle() {
	fd_set m_master;
	std::string name;
	auto quit = false;
	std::cout << "Handling...\n";
	
	while (!quit) {
		FD_ZERO(&m_master);

		auto max_fd = this->registerFileDescriptors(m_master);
		if (max_fd == -1) {
			//There are no clients registered so just wait.
			continue;
		}

		auto retval = select(max_fd + 1, &m_master, NULL, NULL, NULL);
		if (retval == -1) {
			quit = true;
			continue;
		}

		auto clients_sockets = this->serverController.getClientsSockets();
		for (auto it = clients_sockets.begin(); it != clients_sockets.end();) {
			auto client = it->first;
			auto socket = it->second;

			if (FD_ISSET(socket->getSocket(), &m_master)) {
				char buffer[128] = {0};
				auto error = socket->recv(buffer, sizeof(buffer));
				if (error <= 0) {
					std::cerr << "ERROR: Failed to receive from client\n";
					
					FD_CLR(socket->getSocket(), &m_master);
					close(socket->getSocket());
					it = clients_sockets.erase(it);
					continue;
				}

				std::cout << "Size in bytes received: " << error << "\n";
				std::string message(buffer);
		
				auto command = Command::deserialize(message);
				if (command->getCommandType() == Commands::NAME) {
					auto name_command = dynamic_cast<NameCommand*>(command);
					name = name_command->getName();

					client->changeName(name);
					client->unmute();
					
					std::cout << "Name changed!\n";
				} else if (client->canTalk()) {
					this->serverController.messageEverybody(message);
				}
				
				if (message == "name") {
					quit = 1;
					std::cout << "Okay bye!\n";
					continue;
				}

				std::cout << "Message received!\n"; //Print the name like a prompt and signal client to do the same
			}
			it++;
		}
	}

	std::cout << "Stopping handler!\n";
}

void clientHandlerThread(ClientHandler& handler) {
	handler.handle();
}

