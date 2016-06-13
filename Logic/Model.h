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
    std::string fileName="example3.txt";
    std::string device="komputer";
    std::string timestamp = "2016-06-01 00:00:00";
    int fileSize;
public:
    Model();
    ~Model();
    void setUserName(std::string userName) {
        this->userName = userName;
    }
    void setDevice(std::string device) {
        Model::device = device;
    }

    void setTimestamp(int timestamp) {
    time_t     now;
    struct tm  ts;
    char       buf[80];
    // Format time, "ddd yyyy-mm-dd hh:mm:ss zzz"
    time_t times=timestamp;
    ts = *gmtime( &times);
    strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S\0", &ts);
    //printf("%s\n", buf);
    Model::timestamp = std::string(buf);
    }
    void setFileName(std::string fileName) {
        Model::fileName = fileName;
    }
    void setFileSize(int fileSize) {
        Model::fileSize = fileSize;
    }
    void addUser(std::string name, std::string passHash);
    bool findByUsername(std::string name);
    std::string getPassword(std::string username);
    void saveFile(std::string content);
    bool isActive(std::string username);
    bool findFile();
    int findFileVersionSize();

    void addFile();
};


#endif //UNTITLED_MODEL_H
