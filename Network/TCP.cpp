//
// Created by piotr on 30.04.16.
//

#include <cstring>
#include <iostream>
#include "TCP.h"

Network::TCP::TCP(Network::Connection x): connection(x) {
    maxBlockSize = 50;
}

int Network::TCP::sendFrame(std::shared_ptr<char> data, int size, int flag) {
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
                    Header header = { htonl(flag),htonl(size), htonl(0),htonl(0),htonl(0),htonl(0)};
                    char head[sizeof(Header)];
                    memcpy(head, (char*)&header, sizeof(head));

                    char frame[frameSize];
                    memcpy ( frame, (char*)&header ,sizeof(struct Header));
                    memcpy (frame+24, data.get(),size);
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

char* Network::TCP::receive(int n)
{
    char *buffer = new char [n];
    int p = 0;
    while(p<n) {
        int res = (int) connection.getSocket()->recv(buffer + p, (size_t) n - p, 0);
        if (res <= 0) {
            std::cout << "Client disconnected" << std::endl;
            throw "Client disconnected";
        }
        p += res;

        if (p == n)
            return buffer;
    }
    return nullptr;
}

std::pair<std::shared_ptr<char>, std::shared_ptr<struct Network::TCP::Header>> Network::TCP::receiveFrame() {

    std::cout << "Receive ..." << std::endl;

    fd_set master_set;
    fd_set working_set;
    FD_ZERO(&master_set);
    FD_SET(connection.getInputPipe()->getDescriptor(), &master_set);
    FD_SET(connection.getSocket()->getDescriptor(), &master_set);
    int max_sd = std::max(connection.getInputPipe()->getDescriptor(), connection.getSocket()->getDescriptor());

    std::shared_ptr<struct Header> header(new struct Header);
    char* data = new char[100];

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
                    char * buffer;
                    try {
                        buffer = receive(sizeof(struct Header));
                    } catch (const char* msg) {
                        throw msg;
                    }
                    if(buffer == nullptr)
                    {
                        running = false;
                        break;
                    }
                    header->flag = ntohl(((uint32_t *)buffer)[0]);
                    header->length = ntohl(((uint32_t*)buffer)[1]);
                    header->md51 = ntohl(((uint32_t*)buffer)[2]);
                    header->md52 = ntohl(((uint32_t*)buffer)[3]);
                    header->md53 = ntohl(((uint32_t*)buffer)[4]);
                    header->md54 = ntohl(((uint32_t*)buffer)[5]);

                    data = receive(header->length);
                    std::cout<<std::endl<<header->length<<" bytes received."<<std::endl;
                    running = false;
                    break;
                }
            }
        }
    }

    return std::pair<std::shared_ptr<char>, std::shared_ptr<struct Network::TCP::Header>>
            (std::shared_ptr<char>(data), header);
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
        sendFrame(block, maxBlockSize, 0);
    }
    int rest = size - (frames * maxBlockSize);
    char *frame = new char[maxBlockSize];
    for(int i = 0; i < rest; i ++)
    {
        frame[i] = data.get()[i+frames*maxBlockSize];
    }
    std::shared_ptr<char> block(frame);
    sendFrame(block, rest, 1);

    return 0;
}

std::string Network::TCP::receive() {
    int flag = 0;
    int size = 0;
    char* data;
    do {
        std::pair<std::shared_ptr<char>, std::shared_ptr<struct Header>> frame;
        try {
            frame = receiveFrame();
        } catch (const char* msg) {
            throw msg;
        }
        flag = frame.second->flag;
        char* buffer = new char[size + frame.second->length];
        memcpy(buffer, data, size);
        memcpy(buffer + size, frame.first.get(), frame.second->length);
        size += frame.second->length;
        data = new char[size];
        memcpy(data, buffer, size);
    } while(flag==0);
    return std::string(data,size);
}













