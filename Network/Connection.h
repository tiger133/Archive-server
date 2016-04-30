//
// Created by piotr on 29.04.16.
//

#ifndef UNTITLED_CONNECTION_H
#define UNTITLED_CONNECTION_H

#include <memory>
#include "Socket.h"
#include "../utils/InputPipe.h"
using InputPipe = Utils::InputPipe;
using Socket = Network::Socket;


namespace Network
{
    class Connection
    {
        std::shared_ptr<Socket> socket;
        std::shared_ptr<InputPipe> inputPipe;
    public:
        Connection(std::shared_ptr<Socket> sock, std::shared_ptr<InputPipe>& input);
        std::shared_ptr<Socket> getSocket() const;
        std::shared_ptr<InputPipe> getInputPipe() const;


    };
}



#endif //UNTITLED_CONNECTION_H
