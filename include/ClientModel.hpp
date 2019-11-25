#pragma once

#include <iostream>
#include <string>
#include "SocketLinux.hpp"

typedef unsigned int ClientID;

class Client {
    private:
        ClientID id;
        std::string name;
        bool muted;
        SocketLinux* socket;
    public:
        Client(ClientID id, Socket socket);
        ~Client();
        ClientID getId();
        std::string getName();
        SocketLinux * getSocket();
        void changeName(const std::string name);
        void mute();
        void unmute();
        bool canTalk();

        bool operator== (const Client &c) const;
};