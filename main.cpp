#include <iostream>
#include "Network/Server.h"
#include "View/CommandLine.h"

using namespace std;
using Server = Network::Server;
using Connection = Network::Connection;
using CommandLine = View::CommandLine;


int main() {

    Server server(1252);
    server.setConnectListener([](std::shared_ptr<Connection>& x){
        cout<<"Hello World" << endl;
    });

    server.start();


    CommandLine commandLine(server);

    return 0;
}