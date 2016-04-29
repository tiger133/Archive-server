//
// Created by piotr on 29.04.16.
//

#include <sys/socket.h>
#include <netinet/in.h>
#include <stdexcept>
#include <unistd.h>
#include "Socket.h"

Network::Socket::Socket() {
    descriptor = socket(AF_INET,SOCK_STREAM,0);
    if(descriptor == -1)
        throw std::runtime_error("Cannot create socket");
}

void Network::Socket::bind() {
    struct sockaddr_in x;
    x.sin_family = AF_INET;
    x.sin_addr.s_addr = htonl(INADDR_ANY);
    x.sin_port = htons((uint16_t)port);

    if(bind(descriptor,(struct sockaddr*)&x,sizeof(x))!= 0)
        throw std::runtime_error("Cannot bind the address");
}

void Network::Socket::listen() {
    if(::listen(descriptor,100)!=0)
        throw std::runtime_error("Cannot listen on a given port");
}

void Network::Socket::connect() {

}

void Network::Socket::setPort(int port) {
    this->port = port;
}

void Network::Socket::close() {
    if(::close(descriptor)!=0)
        throw std::runtime_error("Cannot close a socket");
}











