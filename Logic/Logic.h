//
// Created by joanna on 27.05.16.
//

#ifndef UNTITLED_CONTROLLER_H
#define UNTITLED_CONTROLLER_H


#include "../Security/Security.h"
#include "Model.h"

class Logic {

    enum State
    {
        DISCONNECTED,
        CONNECTED,
        LOGGED
    } state;
    Security security;
    Model *model;
public:
    Logic(Network::Connection x);
    void start();
    void receive();
    int logIn();
    void logOut();
    void sendChallenge();
    void receiveFile(std::string content, int size);
};


#endif //UNTITLED_CONTROLLER_H
