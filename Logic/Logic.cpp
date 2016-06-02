//
// Created by joanna on 27.05.16.
//

#include "Logic.h"
#include "Model.h"

Logic::Logic(Network::Connection x) : security(x) {
    state = DISCONNECTED;
    model = new Model();
}

void Logic::start() {
    while(true) {
        switch(state) {
            case DISCONNECTED:
                logIn();
                break;
            case CONNECTED:
                logIn();
                break;
            case LOGGED:
                receive();
                break;
        }
        break;
    }
}

void Logic::receive() {
    std::string msg = security.receive();
    int type = msg[0];
    std::string content(msg.data()+1, msg.size()-1);
    switch(type) {
        case 9: receiveFile(content, content.size()); break;
        default: break;
    }
}

int Logic::logIn() {
    std::string msg = security.receive();
    if(msg.at(0) != 1)
        std::runtime_error ("Expected username");
    std::string username(msg.data()+1,msg.size()-1);
    if(model->findByUsername(username)) {
        sendChallenge();
        state = LOGGED;
    }
    else {
        std::cout<<"User: "<< username << " doesn't exist"<<std::endl;
        std::string msg;
        msg.push_back(4);
        msg.push_back(0);
        security.send(msg, 2);
        return -1;
    }

}

void Logic::sendChallenge() {
    static const char hex[] = "0123456789ABCDEF";
    int length = sizeof(hex) - 1;

    srand(time(0));
    std::string challenge;
        for(unsigned int i = 0; i < 8; ++i) {
            challenge += hex[rand() % length];
        }
    std::cout<<challenge<<std::endl;
    std::string withHeader = "1" +challenge;
    security.send(challenge, 9);
}

void Logic::receiveFile(std::string content, int size) {
    model->saveFile(content, size);
}















