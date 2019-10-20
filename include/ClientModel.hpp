#pragma once

#include <iostream>
#include <string>

typedef unsigned int ClientID;

class Client {
    private:
        ClientID id;
        std::string name;
        bool muted;
    public:
        Client(ClientID id);
        ~Client();
        ClientID getId();
        std::string getName();
        void changeName(const std::string name);
        void mute();
        void unmute();
        bool canTalk();

        bool operator== (const Client &c) const;
};