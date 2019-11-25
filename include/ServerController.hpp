#pragma once

#include <memory>
#include <unordered_map>
#include <vector>

#include "ClientModel.hpp"
#include "Types.hpp"
#include "SocketLinux.hpp"

class ServerController{
    private:
        std::vector<Client*> clients;
    
    public:
        ServerController();
        ~ServerController();
        void registerClient(Client *client);
        bool sendMessageClient(std::string name, const Message message);
        Client* getClientByName(std::string name);
        std::vector<Client*> getClients();
        void unregisterClient(Client *c);
        std::vector<std::string> getClientsNames();
};