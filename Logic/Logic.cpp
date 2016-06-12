//
// Created by joanna on 27.05.16.
//

#include "Logic.h"

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
    if(msg.at(0) != 0)
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
        if(response.compare(md5string)==0) {
            if(model->isActive(username)) {
                state = LOGGED;
                model->setUserName(username);
                std::string msg;
                msg.push_back(3);
                security.send(msg, 1);
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
    srand(time(0));
    std::string challenge(8,0);
        for(unsigned int i = 0; i < 8; ++i) {
            challenge[i] = rand() % 256;
        }
    std::cout<<challenge<<std::endl;//another errors
    std::string withHeader = "\001" +challenge;
    security.send(withHeader, 9);
    return challenge;
}

void Logic::receiveFile(std::string content) {
    model->saveFile(content);
}

void Logic::receiveFileRequest(std::string content) {
    char* msg = (char*)content.data();
    std::string fileName = "";
    std::string device = "";
    int i = 0;
    while(msg[i]!='\0') {
        fileName += msg[i];
        i++;
    }
    i++;
    while(msg[i]!='\0') {
        device += msg[i];
        i++;}

    std::cout<<fileName <<", "<<device <<std::endl;
    i++;
    int32_t size =*((int*)(msg+i));
    size=ntohl(size);
    int32_t timestamp = *((int*)(msg+i+4));
    timestamp=ntohl(timestamp);
    
    model->setFileName(fileName);
    model->setDevice(device);
    model->setFileSize(size);
    model->setTimestamp(std::to_string(timestamp));
    if(!model->findFile()) {
        model->addFile();
    }
    if(model->findFileVersionSize() == -1) //nie ma takiej wersji pliku, zgoda na przesyłanie
    {
        std::string msg;
        msg.push_back(7);
        msg.push_back(0);
        msg.push_back(0);
        msg.push_back(0);
        msg.push_back(0);
        security.send(msg, 2);
    }
    else {
        int n = model->findFileVersionSize();
        if(n == size) { // jest już taki plik i cały został przesłany, odmowa przesłania
            std::string msg;
            msg.push_back(8);
            msg.push_back(0);
            security.send(msg, 2);
        }
        else { // jest już taki plik ale nie w całości, zgoda na przesyłanie
            std::string msg;
            msg.push_back(7);
            msg += (char)n>>24;
            msg += (char)n>>16;
            msg += (char)n>>8;
            msg += (char)n;
            security.send(msg, msg.size());
        }
    }

}

void Logic::receiveFileRequest(std::string content) {
   

   // model->setFileName(data.at(0));
  // std::cout << data[0]<<std::endl;
  // std::cout << data[1]<<std::endl;
   // model->setDevice(data.at(1));
   // int size = ntohl(std::stoi(data.at(2)));
   // std::cout <<size<<std::endl;
   // std::cout <<data[3]<<std::endl;
   std::cout << size;
   std::cout << timestamp;
   std::to_string(timestamp);
  //  model->setFileSize(size);
  //  model->setTimestamp(data.at(3));
    if(1) //nie ma takiej wersji pliku, zgoda na przesyłanie
    {
        
    }
    

}

void Logic::logOut() {

}

std::string Logic::md5(std::string message) {
    byte const* pbData = (byte*) message.data();
    unsigned int nDataLen = message.size();
    byte abDigest[CryptoPP::Weak::MD5::DIGESTSIZE];

    CryptoPP::Weak::MD5().CalculateDigest(abDigest, pbData, nDataLen);

    return std::string((char*)abDigest,CryptoPP::Weak::MD5::DIGESTSIZE);
}





















