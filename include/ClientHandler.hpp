#pragma once

#include <memory>
#include <thread>
#include <unordered_map>

#include "ClientModel.hpp"
#include "ServerController.hpp"
#include "SocketBase.hpp"

class ClientHandler{
    private:
        ServerController &serverController;
        Socket registerFileDescriptors(fd_set &master);
    public:
        ClientHandler(ServerController& serverController);
        void handle();

};

void clientHandlerThread(ClientHandler& handler);