#include "Chat.h"
#include "Database.h"

void Chat::checkRequest(string& clmess)
{
    if(clmess[0] == 'R'&& clmess[1]=='%')
    {
        this->transformMess(clmess);
        dbPTR->registerUser(fromClient);
        sendToServ = dbPTR->getAnswer();
    }
        
    if(clmess[0] == 'M'&& clmess[1]=='%')
    {
        this->transformMess(clmess);
        dbPTR->addMessage(fromClient);
        sendToServ = dbPTR->getAnswer();
    }
    
    if(clmess[0] == 'L' && clmess[1] == '%')
    {
        this->transformMess(clmess);
        dbPTR->checkLogin(fromClient);
        sendToServ = dbPTR->getAnswer();
    }

    if(clmess[0] == 'C' && clmess[1] == '%')
    {
        this->transformMess(clmess);

        if(dbPTR->checkName(fromClient))
            sendToServ = "true";
        sendToServ = "false";
    }

    if(clmess[0] == 'S'&& clmess[1]=='%')
    {
        this->transformMess(clmess);
        dbPTR->getAllMessages(fromClient[0]);
        sendToServ = dbPTR->getAnswer();
    }

    if(clmess[0] == 'U' && clmess[1] == '%')
    {
        dbPTR->getAllUsers();
        sendToServ = dbPTR->getAnswer();
    }

    if(clmess[0] == 'D' && clmess[1] == '%')
    {
        this->transformMess(clmess);
        dbPTR->delMessage(fromClient[0]);
        sendToServ = dbPTR->getAnswer();
    }

    if (clmess == "exit") {
        cout << "Client wos disconected!" << endl;
        servPTR->dataReceivingServer();
    }
}

void Chat::transformMess(string& message)
{
    fromClient.clear();
    int start{0}, stop{0};

    start = message.find("%")+1;
    stop = message.find("#");
    while (start != 0)
    {
        fromClient.push_back(message.substr(start, stop - start));
        start = stop + 1;
        stop = message.find("#", start);
    }
}

Chat::~Chat(){}