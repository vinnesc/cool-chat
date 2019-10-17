#pragma once

#include <iostream>
#include <string>

#define CLIENT_ID unsigned int

class Client
{
private:
    CLIENT_ID id;
    int socket;
    std::string name;
    bool muted;
public:
    Client(int id, int socket);
    ~Client();
    CLIENT_ID get_id();
    int get_socket();
    void change_name(std::string name);
    void mute();
    void unmute();
    bool can_talk();

    bool operator== (const Client &c) const;
};

Client::Client(int id, int socket) {
    this->id = id;
    this->socket = socket;
    this->muted = true;
}

Client::~Client() {
}

CLIENT_ID Client::get_id() {
    return this->id;
}

int Client::get_socket() {
    return this->socket;
}

void Client::change_name(std::string name) {
    this->name = name;
}

void Client::mute() {
    this->muted = true;
}

void Client::unmute() {
    this->muted = false;
}

bool Client::can_talk() {
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