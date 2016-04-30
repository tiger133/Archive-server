#include <iostream>
#include "Network/Server.h"
#include "View/CommandLine.h"

using namespace std;
using Server = Network::Server;
using Connection = Network::Connection;
using CommandLine = View::CommandLine;


int main() {

    Server server(1258);
    server.setConnectListener([](std::shared_ptr<Connection>& x){
        for(int i = 0 ;i < 6; i++)
        {
            cout<<i<< endl;
            sleep(1);
        }

    });

    server.start();


    CommandLine commandLine(server);

    return 0;
}