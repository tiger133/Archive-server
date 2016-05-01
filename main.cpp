#include <iostream>
#include "Network/Server.h"
#include "View/CommandLine.h"
#include "Network/TCP.h"
using namespace std;
using Server = Network::Server;
using Connection = Network::Connection;
using CommandLine = View::CommandLine;
using TCP = Network::TCP;

int main() {

    Server server(1266);
    server.setConnectListener([](std::shared_ptr<Connection>& x){
        TCP tcp(*x);
        std::cout<< tcp.receive().get() << std::endl;
    });

    server.start();

    CommandLine commandLine(server);
    return 0;
}