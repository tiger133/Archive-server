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
            u_int64_t md51;
            u_int64_t md52;
            int length;
            int flag;
        };
    public:
        void setMaxBlockSize(int maxBlockSize) {
            TCP::maxBlockSize = maxBlockSize;
        }
        TCP(Connection x);
    private:
        int send(std::shared_ptr<char> data, int size, int flag);
        std::shared_ptr<char> receive();
    public:
        int send(std::shared_ptr<char> data, int size);


    };
}

#endif //UNTITLED_TCP_H
