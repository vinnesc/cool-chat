#pragma once

#include <memory>
#include <thread>

#include "ClientModel.hpp"
#include "ServerController.hpp"

class ClientHandler{
    private:
        std::shared_ptr<Client> client;
        ServerController *serverController;
    public:
        ClientHandler(std::shared_ptr<Client> client, ServerController *serverController);
        void handle();
};

void clientHandlerThread(std::shared_ptr<ClientHandler> handler);