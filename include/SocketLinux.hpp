#pragma once

#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <memory>
#include <iostream>
#include "Types.hpp"

typedef in_port_t Port;
typedef int Socket;

class SocketLinux {
    private:
        Socket listeningSocket;
    	struct sockaddr_in server_address, client_address;
    public:
        SocketLinux(Port port);
        SocketLinux(Socket socket);
        ~SocketLinux();
        Socket getSocket();
        int bind();
        int listen();
        std::shared_ptr<SocketLinux> accept();
        int send(const Message);
        int recv(Buffer, const Length);
};

