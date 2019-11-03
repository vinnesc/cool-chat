#pragma once

#include <memory>
#include <thread>
#include <unordered_map>

#include "ClientModel.hpp"
#include "ServerController.hpp"
#include "SocketBase.hpp"
#include "Command.hpp"

class ClientHandler{
    private:
        ServerController &serverController;
        Socket registerFileDescriptors(fd_set &master);
    public:
        ClientHandler(ServerController& serverController);
        void handle();
        Message handleCommand(std::unique_ptr<Command> &command, std::shared_ptr<Client> &client);

};

void clientHandlerThread(ClientHandler& handler);