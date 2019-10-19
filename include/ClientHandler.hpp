#pragma once

#include <memory>

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