#pragma once
#include <memory>
#include "ServerChat.h"
#include "Database.h"

class Chat
{
private:
    string sendToServ;
    vector<string> fromClient;
    shared_ptr<Chat> chatPTR;
    shared_ptr<Database> dbPTR;
    shared_ptr<Server> servPTR;

public:

    void setToSend(string& send){sendToServ = send;}
    void setChatPtr(Chat chat){chatPTR = make_shared<Chat>(chat);}
    void setDbPtr(Database db){dbPTR = make_shared<Database>(db);}
    void setServPtr(Server sv){servPTR = make_shared<Server>(sv);}
    void showMess(string& name);
    void showUsers(string& mess);
    const string& getToSend(){return this->sendToServ;}
    shared_ptr<Chat>& getChatPtr(Chat chat){return chatPTR;}
    shared_ptr<Database>& getDbPtr(Database db){return dbPTR;}
    shared_ptr<Server>& getServPtr(Server serv){return servPTR;}
    void transformMess(string& message);
    void checkRequest(string& clmess);

    ~Chat();
};