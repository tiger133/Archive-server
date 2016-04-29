//
// Created by piotr on 29.04.16.
//

#include "CommandLine.h"


View::CommandLine::CommandLine(Sever &server)
{
    cout<<"Archive server CommandLine 2.0"<<endl;
    string command;
    while(true)
    {
        cout<<EncourageSign;
        std::getline(cin,command);
        if(command.compare("shutdown")==0)
        {
            server.stop();
            break;
        }

    }
}

