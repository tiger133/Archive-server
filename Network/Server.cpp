//
// Created by piotr on 29.04.16.
//

#include <iostream>
#include "Server.h"

Network::Server::Server(int port) {

}
void Network::Server::setConnectListener(std::function<void(std::shared_ptr<Connection>&)> connectListener) {
    listener = connectListener;
}

void Network::Server::start() {
    serverThread = new std::thread(&Server::run,this);
}


void Network::Server::stop() {

}


void Network::Server::run() const {

}

Network::Server::~Server() {
    serverThread->join();
}












