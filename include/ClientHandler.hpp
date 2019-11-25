#pragma once

#include <memory>
#include <thread>
#include <unordered_map>

#include "ClientModel.hpp"
#include "ServerController.hpp"
#include "SocketLinux.hpp"
#include "Command.hpp"

class ClientHandler{
    private:
        ServerController &serverController;
        SocketLinux& listeningSocket;
        Socket registerFileDescriptors(fd_set &master);
    public:
        ClientHandler(ServerController& serverController, SocketLinux& listeningSocket);
        void handle();
        Message handleCommand(std::unique_ptr<Command> &command, Client &client);

};

void clientHandlerThread(ClientHandler& handler);