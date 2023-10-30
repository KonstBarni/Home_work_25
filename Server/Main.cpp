#include <iostream>
#include "ServerChat.h"
#include "Chat.h"
#include "Database.h"

int main()
{
    system("chcp 1251");

    std::string serverMessage, clientMessage;

    Server server;
    Chat chat;
    Database chatdb{};
    server.createSocket();
    server.bindSocket();
    server.dataReceivingServer();

    while (true) {
        chatdb.connectDatabase();
        chat.setDbPtr(chatdb);
        chat.setServPtr(server);
        chatdb.setServPtrDB(server);
        clientMessage = server.readData();

        if(clientMessage == "stop")
            break;

        chat.checkRequest(clientMessage);
        server.writeData(chat.getToSend());
        
        chatdb.disconnect();
    }

    server.closeSocket();

    return 0;
}