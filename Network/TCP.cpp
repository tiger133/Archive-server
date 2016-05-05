//
// Created by piotr on 30.04.16.
//

#include <cstring>
#include <iostream>
#include "TCP.h"

Network::TCP::TCP(Network::Connection x): connection(x) {
    maxBlockSize = 4;
}

int Network::TCP::send(std::shared_ptr<char> data, int size, int flag) {
    fd_set master_writing_set;
    fd_set master_reading_set;
    fd_set writing_set;
    fd_set reading_set;
    FD_ZERO(&master_writing_set);
    FD_ZERO(&master_reading_set);
    FD_SET(connection.getInputPipe()->getDescriptor(), &master_reading_set);
    FD_SET(connection.getSocket()->getDescriptor(), &master_writing_set);
    int max_sd = std::max(connection.getInputPipe()->getDescriptor(), connection.getSocket()->getDescriptor());

    bool running = true;
    while(running) {
        memcpy(&writing_set, &master_writing_set, sizeof(master_writing_set));
        memcpy(&reading_set, &master_reading_set, sizeof(master_reading_set));
        int result = select(max_sd + 1, &reading_set, &writing_set, NULL,(struct timeval*)0);
        if(result < 0) {
            std::cout << "Select Error" << std::endl;
            break;
        }

        int desc_ready = result;
        for(int i = 0 ; i <= max_sd && desc_ready > 0;++i) {
            if(FD_ISSET(i, &reading_set)) {
                desc_ready -= 1;
                if(i == connection.getInputPipe()->getDescriptor()) {
                    std::cout<<"closing client" << std::endl;
                    running = false;
                    break;
                }
            }
            if(FD_ISSET(i, &writing_set)) {
                desc_ready -= 1;
                if(i == connection.getSocket()->getDescriptor()) {
                    int frameSize = sizeof(Header)+size;

                    std::cout<<"Sending..."<<std::endl;
                    Header header = {htonl(size), htonl(flag), htonl(0),htonl(0),htonl(0),htonl(0)};
                    char head[sizeof(Header)];
                    memcpy(head, (char*)&header, sizeof(head));

                    char frame[frameSize];
                    for(int i = 0 ;i < frameSize + 1;i++)
                        frame[i] = '\0';
                    strcpy(frame, (char*)&header);
                    strcpy(frame+24, data.get());

                    int result = connection.getSocket()->send(frame, (size_t)frameSize, 0);
                    if(result == frameSize)
                    {
                        std::cout<< frameSize <<" bytes sent." << std::endl;
                        running = false;
                        break;
                    }
                }
            }
        }
    }

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
                                char * buffer2 = new char[sizeof(struct Header)];
                                for(int i = 0; i< sizeof(struct Header);i++)
                                {
                                    buffer2[i] = ntohs(buffer[i]);
                                }
                                    //*(buffer+i) = ntohs(*(buffer+i));
                                header = std::shared_ptr<struct Header>((struct Header *) buffer2);
                                std::cout<<header->length<<std::endl;
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

int Network::TCP::send(std::shared_ptr<char> data, int size) {
    int frames = 0;
    while(maxBlockSize < size - (frames * maxBlockSize))
    {
        char *frame = new char[maxBlockSize];
        for(int i = 0; i < maxBlockSize; i ++)
        {
            frame[i]=data.get()[i+frames*maxBlockSize];
        }
        frames++;
        std::shared_ptr<char> block(frame);
        send(block, maxBlockSize, 0);
    }
    int rest = size - (frames * maxBlockSize);
    char *frame = new char[maxBlockSize];
    for(int i = 0; i < rest; i ++)
    {
        frame[i] = data.get()[i+frames*maxBlockSize];
    }
    std::shared_ptr<char> block(frame);
    send(block, rest, 1);

    return 0;
}











