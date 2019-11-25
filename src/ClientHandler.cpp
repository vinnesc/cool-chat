#include <iostream>
#include <cstring>
#include <unistd.h>

#include "ClientHandler.hpp"
#include "Command.hpp"
#include "commands/NameCommand.hpp"
#include "commands/QuitCommand.hpp"
#include "commands/WhisperCommand.hpp"
#include "commands/ListUsersCommand.hpp"

ClientHandler::ClientHandler (ServerController& serverController, SocketLinux& listeningSocket) 
: serverController(serverController), listeningSocket(listeningSocket)
{
}

Socket ClientHandler::registerFileDescriptors(fd_set& master) {
	Socket max = this->listeningSocket.getSocket();
	FD_SET(this->listeningSocket.getSocket(), &master);
	auto clients = this->serverController.getClients();
	
	for (auto& client : clients) {
		auto socket = client->getSocket()->getSocket();
		std::cout << socket << "\n";
		FD_SET(socket, &master);
		
		if (socket > max) {
			max = socket;
		}
	}
	return max;
}

Message ClientHandler::handleCommand(std::unique_ptr<Command> &command, Client *client) {
	std::cout << "Handling command!\n";
	Message response;
	
	switch (command->getCommandType()) {
	case Commands::NAME: 
	{
		auto name_command = dynamic_cast<NameCommand*>(command.get());
		auto name = name_command->getName();

		client->changeName(name);
		client->unmute();
					
		std::cout << "Name changed!\n";
	} break;

	case Commands::WHISPER:
	{
		auto whisper_command = dynamic_cast<WhisperCommand*>(command.get());
		auto receiver = whisper_command->getReceiver();
		auto message = whisper_command->getMessage();

		auto success = this->serverController.sendMessageClient(receiver, message);

		if (!success) {
			std::cerr << "Failed to send message to " << receiver << "\n";
		}
	} break;

	case Commands::LIST_USERS:
	{
		if (dynamic_cast<ListUsersCommand*>(command.get())) {
			//Construct response
			auto names = this->serverController.getClientsNames();
			auto response_command = ListUsersCommand(names);

			response = response_command.serialize();
		}
	} break;
	};

	return response;
}

void ClientHandler::handle() {
	fd_set m_master;
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
			perror("select");
			continue;
		}

		if (FD_ISSET(this->listeningSocket.getSocket(), &m_master)) {
			auto new_socket = this->listeningSocket.accept();
			if (new_socket == -1) {
				std::cerr << "ERROR: unable to accept an incoming connection\n";
				quit = true;
				continue;
			}
			
			std::cout << "New connection!\n";
			Client *client = new Client(0, new_socket);	//TO-DO: Implement clients ID
			serverController.registerClient(client);
		}

		auto clients = this->serverController.getClients();
		for (auto client : clients) {
			auto socket = client->getSocket();

			if (FD_ISSET(socket->getSocket(), &m_master)) {
				char buffer[128] = {0};
				auto error = socket->recv(buffer, sizeof(buffer));
				if (error <= 0) {
					if (errno != EWOULDBLOCK) {
						std::cerr << "ERROR: Failed to receive from client\n";

						FD_ZERO(&m_master);
						close(socket->getSocket());
						this->serverController.unregisterClient(client);
						continue;
					} else {
						continue;
					}
				}
				std::cout << "Size in bytes received: " << error << "\n";

				std::string message(buffer);
				auto command = Command::deserialize(message);
				
				auto response = this->handleCommand(command, client);
				if (response.length() != 0) {
					socket->send(response);
				}
				
				std::cout << "Message received!\n"; //Print the name like a prompt and signal client to do the same
			}
		}
	}

	std::cout << "Stopping handler!\n";
}

void clientHandlerThread(ClientHandler& handler) {
	handler.handle();
}

