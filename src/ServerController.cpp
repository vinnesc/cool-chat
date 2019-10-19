#include <sys/socket.h>

#include "ServerController.hpp"

ServerController::ServerController() {
    this->clients = new std::list< std::shared_ptr<Client> >();
}

ServerController::~ServerController() {
    delete this->clients;
}

bool ServerController::addClient(std::shared_ptr<Client> client) {
    this->clients->push_back(client);

    return true;
}

bool ServerController::removeClient(std::shared_ptr<Client> client) {
    this->clients->remove(client);

    return true;
}

std::shared_ptr<Client> ServerController::getClientByName(const std::string name) {
    for (auto c : *(this->clients)) {
        if (c->getName() == name) {
            return c;
        }
    }

    //This should throw an exception instead?
    return nullptr;
} 

bool ServerController::sendMessageClient(std::shared_ptr<Client> client, const std::string message) {
	auto error = send(client->getSocket(), message.c_str(), message.length(), 0);

	return (bool)error;
}