#include <sys/socket.h>
#include <algorithm>

#include "ServerController.hpp"

ServerController::ServerController() {
}

ServerController::~ServerController() {
    for (auto c : clients) {
        delete c;
    }
}

void ServerController::registerClient(Client *client) {
    this->clients.push_back(client);
}

Client * ServerController::getClientByName(const std::string name) {
    for (auto& c : this->clients) {
        if (c->getName() == name) {
            return c;
        }
    }

    return nullptr;
} 

bool ServerController::sendMessageClient(std::string name, const Message message) {
    auto client = this->getClientByName(name);
    if (client == nullptr) {
        return false;
    }

    auto socket = client->getSocket();

    auto error = socket->send(message);

    if (error < message.length()) {
        return false;
    }

    return true;
}

std::vector<Client*> ServerController::getClients() {
    return this->clients;
}

std::vector<std::string> ServerController::getClientsNames() {
    std::vector<std::string> names;
    for (auto& c : this->clients) {
        names.push_back(c->getName());
    }

    return names;
}

void ServerController::unregisterClient(Client *c) {
    if (c != nullptr) {
        this->clients.erase(std::remove(this->clients.begin(), this->clients.end(), c), this->clients.end());
        delete c;
    }
}
