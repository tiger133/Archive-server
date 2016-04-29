//
// Created by piotr on 29.04.16.
//

#include "Connection.h"


Network::Connection::Connection(std::shared_ptr<Socket> sock, std::shared_ptr<InputPipe> input):socket(sock), inputPipe(input) {

}

