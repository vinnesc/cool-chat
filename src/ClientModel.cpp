#include "ClientModel.hpp"

Client::Client(int id, int socket) {
    this->id = id;
    this->socket = socket;
    this->muted = true;
}

Client::~Client() {
}

CLIENT_ID Client::getId() {
    return this->id;
}

int Client::getSocket() {
    return this->socket;
}

std::string Client::getName() {
    return this->name;
}

void Client::changeName(std::string name) {
    this->name = name;
}

void Client::mute() {
    this->muted = true;
}

void Client::unmute() {
    this->muted = false;
}

bool Client::canTalk() {
    return !this->muted;
}

bool Client::operator== (const Client &c) const
{
    if(c.socket == this->socket) {
        return true;
    } else {
        return false;
    }
}