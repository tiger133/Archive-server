//
// Created by piotr on 29.04.16.
//

#ifndef UNTITLED_SERVER_H
#define UNTITLED_SERVER_H

#include <functional>
#include <memory>
#include "Connection.h"
#include <thread>
#include <netinet/in.h>
#include <vector>
#include "../utils/PipeFactory.h"
#include "../utils/ConcurrentMap.h"


using PipePair = Utils::PipePair;
using OutputPipe = Utils::OutputPipe;
using InputPipe = Utils::InputPipe;
using PipeFactory = Utils::PipeFactory;

namespace Network
{
    class Server
    {
    public:
        Server(int port);
        void start();
        void stop();
        void setConnectListener(std::function<void(std::shared_ptr<Network::Connection>&)> connectListener);
        ~Server();
    private:
        std::shared_ptr<InputPipe> inputPipe;
        std::shared_ptr<OutputPipe> outputPipe;

        Socket serverSocket;
        std::thread * serverThread;
        std::vector<std::shared_ptr<std::thread>> workingThreads;
        Utils::ConcurrentMap<int,std::shared_ptr<OutputPipe>> map;
        std::function<void(std::shared_ptr<Connection>&)> listener;
        void run();

        void createConnection(int desc, sockaddr_in in);
    };


}


#endif //UNTITLED_SERVER_H
