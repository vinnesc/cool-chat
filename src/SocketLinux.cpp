#include "SocketLinux.hpp"

typedef struct sockaddr SA;

SocketLinux::SocketLinux(Port port) {
    this->listeningSocket = ::socket(AF_INET, SOCK_STREAM, 0);
    if (this->listeningSocket == -1) {
        throw "Failed to create a socket";
    }

    this->server_address.sin_family = AF_INET;
	this->server_address.sin_addr.s_addr = htonl(INADDR_ANY);
	this->server_address.sin_port = htons(port);
}

SocketLinux::SocketLinux(Socket socket) {
    if (socket == -1) {
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
    auto retval = ::accept(this->listeningSocket, reinterpret_cast<SA*>(&(this->client_address)), &client_address_length);

    if (retval == -1) {
        return nullptr;
    }

    return std::make_shared<SocketLinux>(retval);
}

int SocketLinux::send(const Message msg) {
    return ::send(this->listeningSocket, msg.c_str(), msg.length(), 0);
}

int SocketLinux::recv(Buffer buf, const Length length) {
    return ::recv(this->listeningSocket, buf, length, 0);
}