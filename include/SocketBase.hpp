#pragma once

#include <iostream>
#include <memory>

typedef int Socket;
typedef char * Buffer;
typedef std::string Message;
typedef size_t Length;

class SocketBase {
    public:
        virtual int bind() = 0;
        virtual int listen() = 0;
        virtual std::shared_ptr<SocketBase> accept() = 0;
        //TO-DO: Make message something general so that we can also send binary data. Add flags.
        virtual int send(const Message) = 0;
        virtual int recv(Buffer, const Length) = 0;
        virtual Socket getSocket() = 0;
};