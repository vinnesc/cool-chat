#pragma once

#include <memory>
#include <unordered_map>

#include "ClientModel.hpp"
#include "SocketBase.hpp"
#include "Types.hpp"

class ServerController{
    private:
        std::unordered_map<std::shared_ptr<Client>, std::shared_ptr<SocketBase> > clients; //abstract collection
    
    public:
        ServerController();
        ~ServerController();
        void addClient(std::shared_ptr<Client> client, std::shared_ptr<SocketBase>);
        bool removeClient(std::shared_ptr<Client> client);
        bool sendMessageClient(std::shared_ptr<Client> client, const Message message);
        void messageEverybody(Message message);
        std::shared_ptr<Client> getClientByName(std::string name);
        std::shared_ptr<SocketBase> getSocketFromClient(std::shared_ptr<Client> client);
        std::unordered_map<std::shared_ptr<Client>, std::shared_ptr<SocketBase> >& getClientsSockets();
};