#include "SocketLinux.hpp"

#include <fcntl.h>

typedef struct sockaddr SA;

SocketLinux::SocketLinux(Port port) {
    this->listeningSocket = ::socket(AF_INET, SOCK_STREAM, 0);
    if (this->listeningSocket == -1) {
        perror("ERROR: socket() failed");
        throw "Failed to create a socket";
    }
    
    auto error = fcntl(this->listeningSocket, F_SETFL, O_NONBLOCK);
    if (error == -1) {
        perror("ERROR: fcntl() failed");
        throw "Failed to create a socket";
    }

    this->server_address.sin_family = AF_INET;
	this->server_address.sin_addr.s_addr = htonl(INADDR_ANY);
	this->server_address.sin_port = htons(port);
}

SocketLinux::SocketLinux(Socket socket) {
    if (socket == -1) {
        std::cerr << "ERROR: invalid socket\n";
        throw "Invalid socket";
    }

    this->listeningSocket = socket;
}

SocketLinux::~SocketLinux() {
    close(this->listeningSocket);
}

Socket SocketLinux::getSocket() {
    return this->listeningSocket;
}

int SocketLinux::bind() {
    return ::bind(this->listeningSocket, reinterpret_cast<SA*>(&(this->server_address)), sizeof(this->server_address));
}

int SocketLinux::listen() {
    return ::listen(this->listeningSocket, 0);
}

std::shared_ptr<SocketBase> SocketLinux::accept() {
    socklen_t client_address_length = sizeof(this->client_address);
    auto socket = ::accept(this->listeningSocket, reinterpret_cast<SA*>(&(this->client_address)), &client_address_length);

    if (socket == -1) {
        return nullptr;
    }

    auto error = fcntl(socket, F_SETFL, O_NONBLOCK);
    if (error == -1) {
        perror("ERROR: fcntl() failed");
        return nullptr;
    }


    return std::make_shared<SocketLinux>(socket);
}

int SocketLinux::send(const Message msg) {
    return ::send(this->listeningSocket, msg.c_str(), msg.length(), 0);
}

int SocketLinux::recv(Buffer buf, const Length length) {
    return ::recv(this->listeningSocket, buf, length, 0);
}