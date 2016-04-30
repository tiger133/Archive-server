//
// Created by piotr on 29.04.16.
//

#include <iostream>
#include <unistd.h>
#include <cstring>
#include <sys/socket.h>
#include <type_traits>
#include "Server.h"
#include <errno.h>

Network::Server::Server(int port) {
    std::pair<std::shared_ptr<InputPipe>, std::shared_ptr<OutputPipe>> pipes = PipeFactory::createPipe();
    inputPipe = pipes.first;
    outputPipe = pipes.second;
    serverSocket.setPort(port);
    serverSocket.bind();
    serverSocket.listen();
}
void Network::Server::setConnectListener(std::function<void(std::shared_ptr<Network::Connection>&)> connectListener) {
    listener = connectListener;
}

void Network::Server::start() {
    serverThread = new std::thread(&Server::run,this);
}


void Network::Server::stop() {

    //closing client threads
    map.foreach([] (std::shared_ptr<OutputPipe> p)->void {
        p->write("close");
    });
    //closing acceptor thread
    outputPipe->write("close");
}


void Network::Server::run() {

    fd_set master_set;
    fd_set working_set;
    FD_ZERO(&master_set);
    FD_SET(inputPipe->getDescriptor(),&master_set);
    FD_SET(serverSocket.getDescriptor(),&master_set);
    int max_sd = std::max(inputPipe->getDescriptor(),serverSocket.getDescriptor());

    bool running = true;
    while(running)
    {
        memcpy(&working_set, &master_set, sizeof(master_set));
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
                desc_ready -= 1;
                if(i == inputPipe->getDescriptor())
                {

                    running = false;
                }
                if(i == serverSocket.getDescriptor())
                {
                    int client_desc;
                    do {

                        struct sockaddr_in  cli_addr;
                        bzero((char *) &cli_addr, sizeof(cli_addr));

                        client_desc = accept(serverSocket.getDescriptor(), NULL,
                                             NULL);
                        if (client_desc < 0) {
                            if(errno != EWOULDBLOCK)
                            {
                                std::cout<<"errno"<< std::endl;
                                running = false;
                            }
                            break;
                        }
                        //We have a client descriptor so we can create a thread and start the logic.
                        createConnection(client_desc,cli_addr);

                        max_sd = std::max(max_sd,client_desc);
                    }while(client_desc != -1);
                }
            }
        }
    }
    std::cout<<"Server is closing..."<< std::endl;


}

Network::Server::~Server() {
    for(auto t: workingThreads)
    {
        t->join();
    }
    serverThread->join();
}

void Network::Server::createConnection(int desc, sockaddr_in in) {

    PipePair pipes = PipeFactory::createPipe();
    Connection * connection = new Connection(std::shared_ptr<Socket>(new Socket(desc,in)),pipes.first);
    std::shared_ptr<Connection> conPtr(connection);


    std::pair<int,std::shared_ptr<OutputPipe>> pair(desc,pipes.second);
    map.insert(pair);
    std::cout <<"map size"  << map.size() << std::endl;
    auto fun = std::bind([this,desc](std::shared_ptr<Connection> x) {
        listener(x);
        map.erase(desc);
        std::cout <<"map size"  << map.size() << std::endl;
    },conPtr);

    std::shared_ptr<std::thread> t(new std::thread(fun));
    workingThreads.push_back(t);
}














