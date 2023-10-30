#include "ServerChat.h"
#include "Chat.h"
#include <filesystem>

using namespace std;
namespace fs = filesystem;

void Server::createSocket()
{
    sockert_file_descriptor = socket(AF_INET, SOCK_STREAM, 0);
    if (sockert_file_descriptor == -1) {
        cout << "Creation of Socket failed!" << endl;
        exit(1);
    }
    else {
        cout << "Socket creation was successful!" << endl;
    }
}

void Server::bindSocket()
{
    serveraddress.sin_addr.s_addr = htonl(INADDR_ANY);
    serveraddress.sin_port = htons(PORT);
    serveraddress.sin_family = AF_INET;
    bind_status = bind(sockert_file_descriptor, (struct sockaddr*)&serveraddress, sizeof(serveraddress));
    if (bind_status == -1) {
        cout << "Socket binding failed.!" << endl;
        exit(1);
    }
    else {
        cout << "The socket binding was successful!" << endl;
    }
}

void Server::closeSocket()
{
    close(sockert_file_descriptor);
}

void Server::dataReceivingServer()
{
    connection_status = listen(sockert_file_descriptor, 5);

    if (connection_status == -1) {
        cout << "Socket is unable to listen for new connections.!" << endl;
        exit(1);
    }
    else {
        cout << "Server is listening for new connection: " << endl;
    }

    length = sizeof(client);
    connection = accept(sockert_file_descriptor, (struct sockaddr*)&client, &length);

    if (connection == -1) {
        cout << "Server is unable to accept the data from client!" << endl;
        exit(1);
    }
    else {
        cout << "The client has joined!" << endl;
    }
}

char* Server::readData()
{
    bzero(message, MESSAGE_LENGTH);
    read(connection, message, sizeof(message));
    cout << "Data received from client: " << message << endl;
    return message;
}

void Server::writeData(string serverMessage)
{
    bzero(message, MESSAGE_LENGTH);
    ssize_t bytes = write(connection, strcpy(message, serverMessage.c_str()), sizeof(message));

    if (bytes >= 0) {
        cout << "Data successfully sent to the client!" << message << endl;
    }
}