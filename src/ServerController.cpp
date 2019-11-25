#include <sys/socket.h>
#include <algorithm>

#include "ServerController.hpp"

ServerController::ServerController() {
}

ServerController::~ServerController() {
}

void ServerController::registerClient(Client& client) {
    this->clients.push_back(client);
}

const Client& ServerController::getClientByName(const std::string name) {
    for (auto& c : this->clients) {
        if (c.getName() == name) {
            return c;
        }
    }

    throw "No client";
} 

bool ServerController::sendMessageClient(std::string name, const Message message) {
    try
    {
        auto client = this->getClientByName(name);
        auto socket = client.getSocket();

        auto error = socket.send(message);

        if (error < message.length()) {
            return false;
        }

	    return true;
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
        return false;
    }   
}

std::vector<Client> ServerController::getClients() {
    return this->clients;
}

std::vector<std::string> ServerController::getClientsNames() {
    std::vector<std::string> names;
    for (auto& c : this->clients) {
        names.push_back(c.getName());
    }

    return names;
}

void ServerController::unregisterClient(Client &c) {
    this->clients.erase(std::remove(this->clients.begin(), this->clients.end(), c), this->clients.end());
}
