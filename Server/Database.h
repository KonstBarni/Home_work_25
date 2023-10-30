#pragma once
#include <memory>
#include <iostream>
#include <mysql/mysql.h>
#include <string.h>
#include <vector>
#include "ServerChat.h"

using namespace std;

class Database {
private:
    string answer;
    shared_ptr<Server> servPtr;
    vector<string> conversion; 

public:
    MYSQL mysql;
    MYSQL_RES* res;
    MYSQL_ROW row;
    MYSQL_FIELD* field;

    bool connectDatabase();
    void disconnect();
    void checkLogin(vector<string>& fromClient);
    const string& getAnswer(){return this->answer;}
    bool checkName(vector<string>& fromClient);
    bool checkRegister(vector<string>& fromClient);
    bool registerUser(vector<string>& fromClient);
    bool addMessage(vector<string>& fromClient);
    void getAllMessages(string& login);
    void getAllUsers();
    void delMessage(string num);
    void setAnswer(string& ans){answer = ans;};
    void setServPtrDB(Server serv){shared_ptr<Server> servPtr = make_shared<Server>(serv);}
    void convertResToVector(MYSQL_RES* res);
    vector<string>& getConvers(){return conversion;}
   
};
