#include <iostream>
#include "Network/Server.h"
#include "View/CommandLine.h"
#include "Network/TCP.h"
#include "Security/Security.h"
using namespace std;
using Server = Network::Server;
using Connection = Network::Connection;
using CommandLine = View::CommandLine;
using TCP = Network::TCP;

int main() {

    Server server(1266);
    server.setConnectListener([](std::shared_ptr<Connection>& x){
        Security security(*x);

        security.receive();
        security.receive();
        security.receive();
        security.receive();
        security.receive();

       // char *data = new char[6];
        //for(int i = 0 ;i < 6;i++)
          //  data[i] = 'a';
        //data[2] = 'x';
        //std::shared_ptr<char> message (data);

     //   security.send(std::shared_ptr<char>(message), 6);

    });

    server.start();

    CommandLine commandLine(server);
    return 0;
}
