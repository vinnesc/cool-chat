#pragma once

#include <memory>
#include <list>

#include "ClientModel.hpp"

class ServerController{
    private:
        std::list< std::shared_ptr<Client> > *clients; //Create a class that abstracts how clients are stored
    
    public:
        ServerController();
        ~ServerController();
        bool addClient(std::shared_ptr<Client> client);
        bool removeClient(std::shared_ptr<Client> client);
        bool sendMessageClient(std::shared_ptr<Client> client, std::string message);
        std::shared_ptr<Client> getClientByName(std::string name);
};