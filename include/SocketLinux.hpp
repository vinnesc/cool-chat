#pragma once

#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

#include "SocketBase.hpp"

typedef in_port_t Port;

class SocketLinux : public SocketBase {
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
        std::shared_ptr<SocketBase> accept();
        int send(const Message);
        int recv(Buffer, const Length);
};

