//
// Created by piotr on 30.04.16.
//

#include <cstring>
#include <iostream>
#include "TCP.h"

Network::TCP::TCP(Network::Connection x): connection(x) {
}

int Network::TCP::send(char *data, int size) {

    //connection.getSocket()->send(data, size, 0);
    fd_set master_set;
    fd_set working_set;
    FD_ZERO(&master_set);
    FD_SET(connection.getInputPipe()->getDescriptor(),&master_set);
    FD_SET(connection.getSocket()->getDescriptor(),&master_set);
    int max_sd = std::max(connection.getInputPipe()->getDescriptor(),connection.getSocket()->getDescriptor());

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
                if(i == connection.getInputPipe()->getDescriptor())
                {
                    std::cout<<"closing client" << std::endl;
                    running = false;
                    break;
                }
                if(i == connection.getSocket()->getDescriptor())
                {
                    //recevive data
                }
            }
        }
    }

    return 0;
}

std::shared_ptr<char> Network::TCP::receive() {
    return std::shared_ptr<char>();
}





