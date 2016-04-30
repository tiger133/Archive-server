//
// Created by piotr on 29.04.16.
//

#include "Connection.h"


Network::Connection::Connection(std::shared_ptr<Socket> sock, std::shared_ptr<InputPipe> &input): socket(sock), inputPipe(input) {

}

std::shared_ptr<Socket> Network::Connection::getSocket() const {
    return socket;
}

std::shared_ptr<InputPipe> Network::Connection::getInputPipe() const {
    return inputPipe;
}

Network::Connection::Connection(Connection &conn) {
    socket = conn.getSocket();
    inputPipe = conn.getInputPipe();
}







