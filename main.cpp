#include <iostream>
#include <fstream>
#include "Network/Server.h"
#include "View/CommandLine.h"
#include "Network/TCP.h"
#include "Security/Security.h"
#include "Logic/Model.h"
#include "Logic/Logic.h"

using namespace std;
using Server = Network::Server;
using Connection = Network::Connection;
using CommandLine = View::CommandLine;
using TCP = Network::TCP;

int main() {

    Server server(1265);
    server.setConnectListener([](std::shared_ptr<Connection>& x){
        Logic* logic = new Logic(*x);
        logic->receive();
        logic->receive();
        logic->receive();
        logic->receive();
        logic->receive();
        //logic->receiveFile("hello");

    //    TCP tcp(*x);
    //    std::cout << tcp.receive();
/*
        Security security(*x);

        std::cout << security.receive() << std::endl;
        std::cout << security.receive() << std::endl;
        /*
        security.receive();
        security.receive();
        security.receive();
        security.receive();
        std::cout << security.receive() << std::endl;


        char *data = new char[31];
        for(int i = 0 ;i < 31;i++)
            data[i] = 'a';
        data[2] = 'x';
        data[14] = '!';
        data[30] = 'B';
        std::shared_ptr<char> message (data);

        security.send(std::shared_ptr<char>(message), 31);
         */
    });

    server.start();

    CommandLine commandLine(server);
    return 0;
}
