//
// Created by piotr on 29.04.16.
//

#ifndef UNTITLED_CONNECTION_H
#define UNTITLED_CONNECTION_H

#include <memory>
#include "Socket.h"
#include "../utils/InputPipe.h"
using InputPipe = Utils::InputPipe;



namespace Network
{
    class Connection
    {
        std::shared_ptr<Socket> socket;
        std::shared_ptr<InputPipe> inputPipe;
    public:
        Connection(std::shared_ptr<Socket> sock,std::shared_ptr<InputPipe> input);



    };
}



#endif //UNTITLED_CONNECTION_H
