#pragma once

#include <memory>
#include <thread>

#include "ClientModel.hpp"
#include "ServerController.hpp"
#include "SocketBase.hpp"

class ClientHandler{
    private:
        std::shared_ptr<Client> client;
        std::shared_ptr<SocketBase> socket;
        ServerController &serverController;
    public:
        ClientHandler(std::shared_ptr<Client> client, std::shared_ptr<SocketBase> socket, ServerController &serverController);
        void handle();
};

void clientHandlerThread(std::shared_ptr<ClientHandler> handler);