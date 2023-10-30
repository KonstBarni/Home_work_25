#include <iostream>
#include "Chat.h"
#include "ClientChat.h"

int main()
{
    std::string clientMessage, serverMessage;
    Chat chat;
    Client client;

    chat.start();
    client.createSocket();
	client.connectionToServer();

    while (chat.isChatWork())
    {
        chat.setSPTR(client);
        chat.showLoginMenu();    

		if (!chat.isChatWork() && chat.isRunServ()) {
            clientMessage ="exit";
            client.writeData(clientMessage);
			break;
		}

        if(!chat.isChatWork() && !chat.isRunServ())
        {
            clientMessage = "stop";
            client.writeData(clientMessage);
            break;
        }

        while (chat.getCurrentUser())
        {
            chat.showUserMenu();
        }

    }
    client.closeSocket();

    return 0;
}