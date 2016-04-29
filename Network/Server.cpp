//
// Created by piotr on 29.04.16.
//

#include <iostream>
#include <unistd.h>
#include <cstring>
#include "Server.h"

Network::Server::Server(int port) {
    std::pair<std::shared_ptr<InputPipe>, std::shared_ptr<OutputPipe>> pipes = PipeFactory::createPipe();
    inputPipe = pipes.first;
    outputPipe = pipes.second;
    serverSocket.setPort(port);
    serverSocket.bind();
    serverSocket.listen();
}
void Network::Server::setConnectListener(std::function<void(std::shared_ptr<Connection>&)> connectListener) {
    listener = connectListener;
}

void Network::Server::start() {
    serverThread = new std::thread(&Server::run,this);
}


void Network::Server::stop() {
    char * buffer = new char[10];
    buffer[0] = 'a';
    outputPipe->write(buffer,1);
}


void Network::Server::run() {

    fd_set master_set;
    fd_set working_set;
    FD_ZERO(&master_set);
    FD_SET(inputPipe->getDescriptor(),&master_set);
    int max_sd = inputPipe->getDescriptor();
    char * buff = new char[1];
    bool running = true;
    while(running)
    {
        memcpy(&working_set,&master_set,sizeof(master_set));
        int result = select(max_sd+1,&working_set,NULL,NULL,(struct timeval*)0);
        if(result < 0)
        {
            std::cout << "Select Error" <<std::endl;
            break;
        }

        int desc_ready = result;
        for(int i = 0 ; i<=max_sd && desc_ready >0;++i)
        {
            if(FD_ISSET(i,&working_set))
            {
                desc_ready = -1;
            }

            if(i == inputPipe->getDescriptor())
            {
                std::cout<<"Server is closing..."<< std::endl;
                running = false;
            }

        }



        sleep(1);
    }


}

Network::Server::~Server() {
    serverThread->join();
}












