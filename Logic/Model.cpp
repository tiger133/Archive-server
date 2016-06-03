//
// Created by joanna on 27.05.16.
//

#include <fstream>
#include "Model.h"
using namespace std;

Model::Model() {
    path = "/home/joanna/ServerFiles/";
    try {
        sql::Driver *driver;

        sql::Statement *stmt;
        sql::ResultSet *res;

        /* Create a connection */
        driver = get_driver_instance();
        con = driver->connect("tcp://127.0.0.1:3306", "root", "haslo1");
        /* Connect to the MySQL test database */
        con->setSchema("files");
        cout << "Connected to database." << endl;

        stmt = con->createStatement();
        res = stmt->executeQuery("SELECT * from user");
        while (res->next()) {
            cout << "\tusername: ";
            /* Access column data by alias or column name */
            cout << res->getString("username") << endl;
        }
    } catch (sql::SQLException &e) {
        cout << "# ERR: SQLException in " << __FILE__;
        cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << endl;
        cout << "# ERR: " << e.what();
        cout << " (MySQL error code: " << e.getErrorCode();
        cout << ", SQLState: " << e.getSQLState() << " )" << endl;
    }
}

void Model::addUser(std::string name, std::string password) {
    sql::PreparedStatement *prep_stmt;

    prep_stmt = con->prepareStatement("INSERT INTO user(active, username, password) VALUES ( 0, ?, ?)");

    prep_stmt->setString(1, name);
    prep_stmt->setString(2, password);
    prep_stmt->execute();
    delete prep_stmt;
}

bool Model::findByUsername(std::string name) {
    sql::PreparedStatement *prep_stmt;

    prep_stmt = con->prepareStatement("SELECT * from user where username = ?");
    prep_stmt->setString(1, name);
    sql::ResultSet *res = prep_stmt->executeQuery();
    if(res->next()) {
        delete prep_stmt;
        return true;
    }
    delete prep_stmt;
    return false;
}

void Model::saveFile(std::string content) {
    std::cout<<"saving file"<<std::endl;
    sql::PreparedStatement *prep_stmt;

    prep_stmt = con->prepareStatement("SELECT fileSize from file join user on file.userId = user.id where username = ?");
    prep_stmt->setString(1, userName);
    sql::ResultSet *res = prep_stmt->executeQuery();
    int fileSize;
    if(res->next()) {
        fileSize = res->getInt("fileSize");
    }
    else {
        std::cout<< "error" << std::endl;
        return;
    }
    delete prep_stmt;


    std::string filePath = path+userName+"/"+device;
    std::string command = "mkdir -p "+filePath;
    system(command.data());

    std::cout<<"filePath: "<<filePath<<std::endl;
    std::ofstream myfile;
    myfile.open(filePath+"/"+fileName, std::fstream::app);
    myfile.write(content.data(), content.size());
    myfile.close();
    std::cout<<"saved"<<std::endl;
}

std::string Model::getPassword(std::string username) {
    sql::PreparedStatement *prep_stmt;

    prep_stmt = con->prepareStatement("SELECT * from user where username = ?");
    prep_stmt->setString(1, username);
    sql::ResultSet *res = prep_stmt->executeQuery();
    if(res->next()) {
        delete prep_stmt;
        return res->getString("password");
    }
    delete prep_stmt;
    return NULL;
}

bool Model::isActive(std::string username) {
    sql::PreparedStatement *prep_stmt;

    prep_stmt = con->prepareStatement("SELECT * from user where username = ?");
    prep_stmt->setString(1, username);
    sql::ResultSet *res = prep_stmt->executeQuery();
    if(res->next()) {
        delete prep_stmt;
        return res->getBoolean("active");
    }
    delete prep_stmt;
    return false;
}











