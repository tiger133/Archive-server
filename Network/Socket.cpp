//
// Created by piotr on 29.04.16.
//

#include <sys/socket.h>
#include <netinet/in.h>
#include <stdexcept>
#include <unistd.h>
#include <sys/ioctl.h>
#include "Socket.h"

Network::Socket::Socket() {
    descriptor = socket(AF_INET,SOCK_STREAM,0);
    if(descriptor < 0)
        throw std::runtime_error("Cannot create socket");
    int on = 1;
    int result = setsockopt(descriptor,SOL_SOCKET,SO_REUSEADDR,(char*)&on,sizeof(on));
    if(result < 0)
        throw std::runtime_error("Cannot setsockopt");
    result = ioctl(descriptor,FIONBIO,(char*)&on);
    if(result < 0)
        throw std::runtime_error("Cannot ioctl");

}

void Network::Socket::bind() {
    struct sockaddr_in x;
    x.sin_family = AF_INET;
    x.sin_addr.s_addr = htonl(INADDR_ANY);
    x.sin_port = htons((uint16_t)port);

    if(::bind(descriptor,(struct sockaddr*)&x,sizeof(x))!= 0)
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

Network::Socket::~Socket() {
    if(::close(descriptor)!=0)
        throw std::runtime_error("Cannot close a socket");
}

int Network::Socket::getDescriptor() {
    return descriptor;
}

Network::Socket::Socket(int desc, sockaddr_in in) : descriptor(desc) , sockaddrIn(in){

}

ssize_t Network::Socket::recv(void *buff, size_t len, int flag) {

    return ::recv(descriptor,buff,len,flag);
}
ssize_t Network::Socket::send(void *buff, size_t len, int flag) {
    return ::send(descriptor,buff,len,flag);
}



















