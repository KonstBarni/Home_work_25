#pragma once
#include <vector>
#include <exception>
#include <memory>
#include "Massage.h"
#include "User.h"
#include "ClientChat.h"

using namespace std;

struct UserLoginExp : public exception		//исключение ошибки логина
{
	const char* what() const noexcept override { return "This user already exist!"; }
};

struct UserNameExp : public exception		//исключение ошибки имени юзера
{
	const char* what() const noexcept override { return "This user already exist!"; }
};

class Chat
{
	bool runServer_ = true;
	bool isChatWork_ = false;						//маркер работы чата

	vector<Message>messages_;						//вектор сообщений
	shared_ptr<User> currentUser_ = nullptr;		//указатель активного пользователя
	vector<string> fromServ;
	string messFromServ;
	shared_ptr<Client> cltptr;

public:

	void login();												//логин
	void registration();										//регистрация
	void showChat();											//показать чат
	void showAllUsersName();									//показать всех пользователей
	void addMessage(shared_ptr<Client> cltptr);					//добавить сообщение
	void dellMessage();											//удалить сообщение
	bool checkName(string& name);
	void transformMess(string& message);
	bool is_work(){return isChatWork_;}
	bool isRunServ(){return runServer_;}
	void setSPTR(Client cl){cltptr = make_shared<Client>(cl);}	// shared_ptr on client
	bool isChatWork() const { return isChatWork_; }				//маркер работы чата
	void start();												//старт чата
	void showLoginMenu();										//меню логина/регистрации
	void showUserMenu();										//меню пользователя
	shared_ptr<User> getCurrentUser() const { return currentUser_; }	//возвращает активного пользователя
	
};