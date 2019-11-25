#pragma once

#include <memory>
#include <unordered_map>
#include <vector>

#include "ClientModel.hpp"
#include "Types.hpp"
#include "SocketLinux.hpp"

class ServerController{
    private:
        std::unordered_map<std::shared_ptr<Client>, std::shared_ptr<SocketLinux> > clients; //abstract collection
    
    public:
        ServerController();
        ~ServerController();
        void addClient(std::shared_ptr<Client> client, std::shared_ptr<SocketLinux>);
        bool removeClient(std::shared_ptr<Client> client);
        bool sendMessageClient(std::string name, const Message message);
        std::shared_ptr<Client> getClientByName(std::string name);
        std::shared_ptr<SocketLinux> getSocketFromClient(std::shared_ptr<Client> client);
        std::unordered_map<std::shared_ptr<Client>, std::shared_ptr<SocketLinux> > getClientsSockets();
        bool unregisterClient(std::shared_ptr<Client> &c);
        std::vector<std::string> getClientsNames();
};