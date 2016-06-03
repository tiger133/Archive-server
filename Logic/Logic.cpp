//
// Created by joanna on 27.05.16.
//
#define CRYPTOPP_ENABLE_NAMESPACE_WEAK 1

#include "Logic.h"
#include "Model.h"
#include "cryptopp/md5.h"

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
        case 9: receiveFile(content); break;
        case 6: receiveFileRequest(content); break;
        case 12: logOut(); break;
        default: break;
    }
}

int Logic::logIn() {
    std::string msg = security.receive();
    if(msg.at(0) != 1)
        std::runtime_error ("Expected username");

    std::string username(msg.data()+1,msg.size()-1);
    if(model->findByUsername(username)) {
        std::string challenge = sendChallenge();
        std::string msg = security.receive();
        if(msg.at(0) != 2)
            std::runtime_error ("Expected response");
        std::string hash = model->getPassword(username) + challenge;
        std::string md5string = md5(hash);
        std::string response(msg.data()+1,msg.size()-1);
        if(strcmp(md5string.data(), response.data())) {
            if(model->isActive(username)) {
                state = LOGGED;
                model->setUserName(username);
            }
            else {
                std::cout<<"User account has not been activated."<<std::endl;
                std::string msg;
                msg.push_back(4);
                msg.push_back(2);
                security.send(msg, 2);
                return -1;
            }
        }
        else {
            std::cout<<"Wrong password"<<std::endl;
            std::string msg;
            msg.push_back(4);
            msg.push_back(1);
            security.send(msg, 2);
            return -1;
        }
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

std::string Logic::sendChallenge() {
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
    return challenge;
}

void Logic::receiveFile(std::string content) {
    model->saveFile(content);
}

void Logic::receiveFileRequest(std::string content) {
    char* msg = (char*)content.data();
    std::string fileName = "";
    std::string device = "";
    std::string tmp = "";
    int i = 0;
    while(msg[i]!='\0') {
        fileName += msg[i];
        i++;
    }
    i++;
    while(msg[i]!='\0') {
        device += msg[i];
        i++;
    }
    std::cout<<fileName <<", "<<device <<std::endl;
    i++;
    while(msg[i]!='\0') {
        tmp += msg[i];
        i++;
    }
    //int32_t size = (int32_t*)tmp.data();
    tmp = "";
    i++;
    while(msg[i]!='\0') {
        tmp += msg[i];
        i++;
    }
    //int32_t timestamp = int32_t(tmp.data());
    model->setFileName(fileName);
    model->setDevice(device);
    //model->setTimestamp(timestamp);

}

void Logic::logOut() {

}

std::string Logic::md5(std::string message) {
    byte digest[ CryptoPP::Weak::MD5::DIGESTSIZE ];

    CryptoPP::Weak::MD5 hash;
    hash.CalculateDigest( digest, (const byte*)message.c_str(), message.length() );

    CryptoPP::HexEncoder encoder;
    std::string output;

    encoder.Attach( new CryptoPP::StringSink( output ) );
    encoder.Put( digest, sizeof(digest) );
    encoder.MessageEnd();

    return output;
}





















