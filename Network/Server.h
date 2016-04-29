//
// Created by piotr on 29.04.16.
//

#ifndef UNTITLED_SERVER_H
#define UNTITLED_SERVER_H

#include <functional>
#include <memory>
#include "Connection.h"
#include <thread>

namespace Network
{
    class Server
    {
    public:
        Server(int port);
        void start();
        void stop();
        void setConnectListener(std::function<void(std::shared_ptr<Connection>&)> connectListener);
        ~Server();
    private:
        std::thread * serverThread;
        std::function<void(std::shared_ptr<Connection>&)> listener;
        void run() const;
    };


}


#endif //UNTITLED_SERVER_H
