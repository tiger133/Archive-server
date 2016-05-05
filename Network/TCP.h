//
// Created by piotr on 30.04.16.
//

#ifndef UNTITLED_TCP_H
#define UNTITLED_TCP_H

#include "Socket.h"
#include "Connection.h"

namespace Network {
    class TCP {
        Connection connection;
        int maxBlockSize;
        enum ReceiveState
        {
            HEADER,
            DATA,
        } receiveState;
        struct Header {
            uint32_t flag;
            uint32_t length;
            uint32_t md51;
            uint32_t md52;
            uint32_t md53;
            uint32_t md54;
        };
    public:
        void setMaxBlockSize(int maxBlockSize) {
            TCP::maxBlockSize = maxBlockSize;
        }
        TCP(Connection x);
    private:
        int send(std::shared_ptr<char> data, int size, int flag);
    public:
        std::shared_ptr<char> receive();

        int send(std::shared_ptr<char> data, int size);


    };
}

#endif //UNTITLED_TCP_H
