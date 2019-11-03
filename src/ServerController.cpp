#include <sys/socket.h>

#include "ServerController.hpp"

ServerController::ServerController() {
}

ServerController::~ServerController() {
}

void ServerController::addClient(std::shared_ptr<Client> client, std::shared_ptr<SocketBase> socket) {
    this->clients.insert(std::make_pair(client, socket));
}

bool ServerController::removeClient(std::shared_ptr<Client> client) {
    return this->clients.erase(client);
}

std::shared_ptr<Client> ServerController::getClientByName(const std::string name) {
    for (auto& c : this->clients) {
        if (c.first->getName() == name) {
            return c.first;
        }
    }

    //This should throw an exception instead?
    return nullptr;
} 

bool ServerController::sendMessageClient(std::string name, const Message message) {
    auto client = this->getClientByName(name);
    auto socket = this->getSocketFromClient(client);

    if (client == nullptr) {
        return false;
    }

    auto error = socket->send(message);

    if (error < message.length()) {
        return false;
    }

	return true;
}

std::shared_ptr<SocketBase> ServerController::getSocketFromClient(std::shared_ptr<Client> client) {
    auto pair = this->clients.find(client);
    
    if (pair == this->clients.end()) {
        return nullptr;
    }

    return pair->second;
}

std::unordered_map<std::shared_ptr<Client>, std::shared_ptr<SocketBase> >& ServerController::getClientsSockets() {
    return this->clients;
}

std::vector<std::string> ServerController::getClientsNames() {
    std::vector<std::string> names;
    for (auto& c : this->clients) {
        names.push_back(c.first->getName());
    }

    return names;
}
