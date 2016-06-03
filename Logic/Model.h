//
// Created by joanna on 27.05.16.
//

#ifndef UNTITLED_MODEL_H
#define UNTITLED_MODEL_H
#include <iostream>
#include "mysql_connection.h"

#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>


class Model {
    std::string path;
    sql::Connection *con;
    std::string userName="piotr";
    std::string fileName="example2.txt";
    std::string device="komputer";
    int timestamp;
public:
    Model();
    ~Model();
    void setUserName(std::string userName) {
        this->userName = userName;
    }
    void setDevice(const std::string &device) {
        Model::device = device;
    }

    void setTimestamp(int timestamp) {
        Model::timestamp = timestamp;
    }

    void setFileName(const std::string &fileName) {
        Model::fileName = fileName;
    }
    void addUser(std::string name, std::string passHash);
    bool findByUsername(std::string name);
    std::string getPassword(std::string username);
    void saveFile(std::string content);
    bool isActive(std::string username);
};


#endif //UNTITLED_MODEL_H
