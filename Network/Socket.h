//
// Created by piotr on 29.04.16.
//

#ifndef UNTITLED_SOCKET_H
#define UNTITLED_SOCKET_H

namespace Network
{
    class Socket
    {
    public:
        Socket();
        void bind();
        void listen();
        void connect();

    };

}


#endif //UNTITLED_SOCKET_H
