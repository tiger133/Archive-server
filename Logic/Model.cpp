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
            cout << "\t... MySQL replies: ";
            /* Access column data by alias or column name */
            cout << res->getString("username") << endl;
            cout << "\t... MySQL says it again: ";
            /* Access column fata by numeric offset, 1 is the first column */
            cout << res->getString(2) << endl;
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

void Model::saveFile(std::string content, int size) {
    std::cout<<"saving file"<<std::endl;
    std::string filePath = path+"asia5.txt";
    std::cout<<filePath<<std::endl;
    std::ofstream myfile;
    myfile.open(filePath);
    myfile << content;
    myfile.close();
    std::cout<<"saved"<<std::endl;
}







