#include "SqliteDataBase.h"



bool SqliteDataBase::open() {
    string dbFileName = "DB";
    char* errmsg = 0;
    int fileExist = _access(dbFileName.c_str(), 0);

    int res = sqlite3_open(dbFileName.c_str(), &this->db);
    if (res != SQLITE_OK) {
        this->db = nullptr;
        cout << "Failed to open DB" << endl;
        return false;
    }
    return true;
}

void SqliteDataBase::close() {
    sqlite3_close(this->db);
}

void SqliteDataBase::clear() {
    remove("DB");
}

bool SqliteDataBase::createUser(string hostName, string password, string ipAdress, string port) {
    int res;
    string sqlStatement = "INSERT INTO Users (HOSTNAME, PASSWORD, IPADRESS, PORT) VALUES ('" + hostName + "','" + password + "', '" + ipAdress + "', '" + port + "');";
    char* errMessage = nullptr;
    res = sqlite3_exec(this->db, sqlStatement.c_str(), nullptr, nullptr, &errMessage);
    if (res != SQLITE_OK) {
        return false;
    }
    return true;

}

vector<std::string> SqliteDataBase::getIpAndPort(string password)
{
    std::vector<std::string> userInfo;
    std::string selectIp = "select IPADRESS from users where password='" + password + "';";
    std::string selectPort = "select PORT from users where password='" + password + "';";
    std::string port, ip;
    int res = 0;
    char* errorMsg = nullptr;

    auto callback = [](void* data, int argc, char** argv, char** colName)
    {
        *((std::string*)data) = argv[0];
        return 0;
    };


    res = sqlite3_exec(db, selectIp.c_str(), callback, &ip, &errorMsg);
    res = sqlite3_exec(db, selectPort.c_str(), callback, &port, &errorMsg);

    return std::vector<std::string>{ ip, port };

}


