//
// Created by piotr on 29.04.16.
//

#ifndef UNTITLED_SERVER_H
#define UNTITLED_SERVER_H

#include <functional>
#include <memory>
#include "Connection.h"

namespace Modules
{
    class Server
    {
    public:
        Server(int port);
        void start();
        void stop();
        void setConnectListener(std::Function<void(std::shared_ptr<Connection> & connection));
    };


}


#endif //UNTITLED_SERVER_H
