//
// Created by piotr on 29.04.16.
//

#ifndef UNTITLED_SOCKET_H
#define UNTITLED_SOCKET_H

namespace Network
{
    class Socket
    {
        int port;
        int descriptor;
    public:
        Socket();
        void bind();
        void listen();
        void connect();
        void close();
        void setPort(int i);
    };

}


#endif //UNTITLED_SOCKET_H
