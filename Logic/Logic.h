//
// Created by joanna on 27.05.16.
//

#ifndef UNTITLED_CONTROLLER_H
#define UNTITLED_CONTROLLER_H


#include "../Security/Security.h"
#include "Model.h"
#include <string>
#include <vector>
#include "cryptopp/md5.h"

class Logic {

    enum State
    {
        DISCONNECTED,
        CONNECTED,
        LOGGED
    } state;
    Security security;

public:
    Model *model;
    Logic(Network::Connection x);
    void start();
    void receive();
    int logIn();
    void logOut();
    std::string sendChallenge();
    void receiveFile(std::string content);
    void receiveFileRequest(std::string content);
    std::string md5(std::string x);
};


#endif //UNTITLED_CONTROLLER_H
