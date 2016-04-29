//
// Created by piotr on 29.04.16.
//

#ifndef UNTITLED_COMMANDLINE_H
#define UNTITLED_COMMANDLINE_H

#include <iostream>
#include "../Network/Server.h"

using namespace std;
using Sever  = Network::Server;

namespace View
{
    class CommandLine
    {
          std::string EncourageSign = "> ";
    public:
        CommandLine(Sever &server);
    };

}


#endif //UNTITLED_COMMANDLINE_H
