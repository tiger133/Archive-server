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
public:
    Model();
    ~Model();
    void addUser(std::string name, std::string passHash);
    bool findByUsername(std::string name);

    void saveFile(std::string content, int size);
};


#endif //UNTITLED_MODEL_H
