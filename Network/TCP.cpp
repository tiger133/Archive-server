//
// Created by piotr on 30.04.16.
//

#include <cstring>
#include <iostream>
#include "TCP.h"

Network::TCP::TCP(Network::Connection x): connection(x) {
}

int Network::TCP::send(char *data, int size) {


}

std::shared_ptr<char> Network::TCP::receive() {

    std::cout << "Receive ..." << std::endl;

    fd_set master_set;
    fd_set working_set;
    FD_ZERO(&master_set);
    FD_SET(connection.getInputPipe()->getDescriptor(), &master_set);
    FD_SET(connection.getSocket()->getDescriptor(), &master_set);
    int max_sd = std::max(connection.getInputPipe()->getDescriptor(), connection.getSocket()->getDescriptor());

    char *buffer = new char [100];
    std::shared_ptr<struct Header> header;
    char *data;

    int hp = 0;
    int dp = 0;
    receiveState = HEADER;

    bool running = true;
    while(running) {
        memcpy(&working_set, &master_set, sizeof(master_set));
        int result = select(max_sd + 1, &working_set, NULL, NULL,(struct timeval*)0);
        if(result < 0) {
            std::cout << "Select Error" << std::endl;
            break;
        }

        int desc_ready = result;
        for(int i = 0 ; i <= max_sd && desc_ready > 0;++i) {
            if(FD_ISSET(i, &working_set)) {
                desc_ready -= 1;
                if(i == connection.getInputPipe()->getDescriptor()) {
                    std::cout<<"closing client" << std::endl;
                    running = false;
                    break;
                }
                if(i == connection.getSocket()->getDescriptor()) {
                    switch(receiveState) {
                        case HEADER:{
                            int res = (int) connection.getSocket()->recv(buffer + hp,
                                                                         (size_t) sizeof(struct Header) - hp, 0);
                            if (res <= 0) {
                                running = false;
                                break;
                            }
                            hp += res;

                            if (hp == sizeof(struct Header)) {
                                receiveState = DATA;
                                header = std::shared_ptr<struct Header>((struct Header *) buffer);

                                data = new char[header->length+1];
                                for(int i = 0 ;i < header->length + 1;i++)
                                    data[i] = '\0';
                            }
                            break;
                            }
                        case DATA: {
                            int res = (int) connection.getSocket()->recv(data + dp, (size_t) header->length - dp, 0);
                            if (res <= 0) {
                                running = false;
                                break;
                            }
                            dp += res;

                            if (dp == header->length) {
                                running = false;
                                break;
                            }
                            break;
                        }
                    }
                }
            }
        }
    }

    return std::shared_ptr<char>(data);
}





