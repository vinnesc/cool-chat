#pragma once

#include <iostream>
#include <string>

#define CLIENT_ID unsigned int

class Client {
    private:
        CLIENT_ID id;
        int socket;
        std::string name;
        bool muted;
    public:
        Client(int id, int socket);
        ~Client();
        CLIENT_ID getId();
        int getSocket();
        std::string getName();
        void changeName(const std::string name);
        void mute();
        void unmute();
        bool canTalk();

        bool operator== (const Client &c) const;
};