//
// Created by piotr on 29.04.16.
//

#ifndef UNTITLED_SOCKET_H
#define UNTITLED_SOCKET_H
#include <netinet/in.h>
namespace Network
{
    class Socket
    {
        int port;
        int descriptor;
        sockaddr_in sockaddrIn;
    public:

        Socket();
        //Socket(int socketDescriptor);
        void bind();
        void listen();
        void connect();
        void close();
        ssize_t recv(void * buff,size_t len, int flag);
        ssize_t send(void * buff, size_t len, int flag);
        void setPort(int i);
        int getDescriptor();
        ~Socket();

        Socket(int i, sockaddr_in in);
    };

}


#endif //UNTITLED_SOCKET_H
