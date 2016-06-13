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

    prep_stmt = con->prepareStatement("SELECT * from file join user on file.user_id = user.id "
                                              "where username = ? and file_name = ? and device_name = ?");
    prep_stmt->setString(1, userName);
    prep_stmt->setString(2, fileName);
    prep_stmt->setString(3, device);
    sql::ResultSet *res = prep_stmt->executeQuery();
    int fileSize;
    int fileId;
    if(res->next()) {
        fileSize = res->getInt("file_size");
        fileId = res->getInt("id");
    }
    else {
        std::cout<< "error" << std::endl;
        return;
    }

    prep_stmt = con->prepareStatement("UPDATE file_version SET received_data_size = ? where file_id =? and timestamp = ?");
    prep_stmt->setInt(1, fileSize + content.size());
    prep_stmt->setInt(2, fileId);
    prep_stmt->setDateTime(3, timestamp);
    prep_stmt->execute();


    std::string filePath = path+userName+"/"+device;
    std::string command = "mkdir -p "+filePath;
    system(command.data());

    std::cout<<"filePath: "<<filePath<<std::endl;
    std::ofstream myfile;
    myfile.open(filePath+"/"+fileName+"_"+timestamp, std::fstream::app);
    myfile.write(content.data(), content.size());
    myfile.close();
    std::cout<<"saved"<<std::endl;
    delete prep_stmt;
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

int Model::findFileVersionSize() {
    sql::PreparedStatement *prep_stmt;

    prep_stmt = con->prepareStatement(
            "SELECT * from file join user on file.userId = user.id "
                    "join file_version on file.id = file_version.fileId "
                    "where username = ? and filename = ? and deviceName = ? and timestamp = ?");
    prep_stmt->setString(1, userName);
    prep_stmt->setString(2, fileName);
    prep_stmt->setString(3, device);
    prep_stmt->setDateTime(4, timestamp);

    sql::ResultSet *res = prep_stmt->executeQuery();
    delete prep_stmt;
    if(res->next()) {
        return res->getInt("receivedDataSize");
    }

    return -1;
}

bool Model::findFile() {
    sql::PreparedStatement *prep_stmt;

    prep_stmt = con->prepareStatement("SELECT * from file join user on file.userId = user.id "
                                              "where username = ? and filename = ? and deviceName = ?");
    prep_stmt->setString(1, userName);
    prep_stmt->setString(2, fileName);
    prep_stmt->setString(3, device);
    sql::ResultSet *res = prep_stmt->executeQuery();
    delete prep_stmt;
    if(res->next()) {
        return true;
    }
    return false;
}

void Model::addFile() {
    sql::PreparedStatement *prep_stmt;

    prep_stmt = con->prepareStatement("SELECT id from user where username = ?");
    prep_stmt->setString(1, userName);
    sql::ResultSet *res = prep_stmt->executeQuery();
    int id = 0;
    if(res->next()) {
        id = res->getInt("id");
    }
    else {
        std::cout<<"No such user: " << userName << std::endl;
        return;
    }
    prep_stmt = con->prepareStatement("INSERT into file(file_name, user_id, file_size, device_name) VALUES (?,?,?,?) ");
    prep_stmt->setString(1, fileName);
    prep_stmt->setInt(2, id);
    prep_stmt->setInt(3, fileSize);
    prep_stmt->setString(4, device);
    std::cout<< prep_stmt->execute() << endl;

    prep_stmt = con->prepareStatement("SELECT * from file where file_name = ? and "
                                              "user_id =? and file_size = ? and device_name = ?");
    prep_stmt->setString(1, fileName);
    prep_stmt->setInt(2, id);
    prep_stmt->setInt(3, fileSize);
    prep_stmt->setString(4, device);
    res = prep_stmt->executeQuery();
    int fileId;
    if(res->next()) {
        fileId = res->getInt("id");
    }
    else {
        std::cout<<"No such file: " << fileName << std::endl;
        return;
    }

    prep_stmt = con->prepareStatement("INSERT into file_version (file_id, timestamp, received_data_size) VALUES (?,?,0) ");
    prep_stmt->setInt(1, fileId);
    prep_stmt->setString(2, timestamp);
    prep_stmt->execute();
    delete prep_stmt;

}

















