#include <iostream>
#include <iterator>
#include <iomanip>
#include <algorithm>
#include "Chat.h"
#include "ClientChat.h"
#include "autocomplite/Functions.h"
#include "Sha1.h"

using namespace std;

void Chat::start()			//запуск работы чата
{
	isChatWork_ = true;	
}

void Chat::login()		//функция логина в чат
{
	string name, login, password, messToServ, messFromServ;
	char operation;
	bool admin;

	const char* pass = password.c_str();
	uint* passHex = sha1(pass, strlen(pass));

	while (!currentUser_)
	{
		again:
		cout << "login: ";
		cin >> login;
		cout << "password: ";
		cin >> password;

		const char* pass = password.c_str();
		uint* passHex = sha1(pass, strlen(pass));

		messToServ = "L%" + login +"#"+ to_string(*passHex);
		cltptr->writeData(messToServ); 
		messFromServ = cltptr->readData();
		cout << messFromServ << endl;

		if (messFromServ == "error")
		{								
			currentUser_ = nullptr;							

			cout << "login failed" << endl << "(q) for exit or (any key) for repeat: ";
			cin >> operation;

			if (operation == 'q')						
				break;
			else
				goto again;
		}
		transformMess(messFromServ);
		name = fromServ[0];
		admin = (fromServ[1]=="0")? false : true;
		User us(login, name, password, admin);
		currentUser_ = make_shared<User> (us);
	}
	

}

void Chat::registration()				//функция регистрации
{
	string login, name, password, sendUser;
	bool admin;
	char adminKey{};

	again:
	cout << "login: ";
	cin >> login;
	cout << "name: ";
	cin >> name;
	cout << "password: ";
	cin >> password;
	cout << "Вы администратор?  [Y/N]  ";
	cin >> adminKey;

	if (adminKey == 'Y' || adminKey == 'y' || adminKey == '1')	//проверка ввода админа
		admin = true;

	const char* pass = password.c_str();
	uint* passHex = sha1(pass, strlen(pass));
	sendUser = "R%" + login + '#' + name + '#' + to_string(*passHex) + '#' + (admin? "true" : "false");
	cltptr->writeData(sendUser);
	string ans = cltptr->readData();
	if(ans == "error")
	{
		cout << "This user already exist"<<endl;
		goto again;
	}
	
	User user = User(login, name, password, admin);
	
	currentUser_ = make_shared<User>(user);			//делает пользователя активным
}

void Chat::showChat()			//показать сообщения чата
{
	string name = currentUser_->getUserName();
	unsigned short count{0};

	cout << "Start Chat" << endl;
	string send = "S%" + name;
	cltptr->writeData(send);
	messFromServ = cltptr->readData();

	if (messFromServ == "empty")
	{
        cout << "empty" << endl;
    }
    else 
	{
		transformMess(messFromServ);
        for (int i = 0; i < fromServ.size()-1 ; i += 4) {
			cout <<setw(4) << left << fromServ[i];
            cout << " from: " << setw(10) << left << fromServ[i+1] <<" ";
            cout << "to: " << setw(10) << left << fromServ[i + 2] <<" ";
            cout << "text: " << setw(30) <<left << fromServ[i + 3] << endl;
        }
    }

}

void Chat::showLoginMenu() 			//меню логина/регистрации
{
	currentUser_ = nullptr;

	char operation;

	cout << "If are you wont stop a server: enter 's'" << endl;

	do
	{
		cout << "(1)Registration" << endl << "(2)Login" << endl << "(q)Quit program" <<
		endl << "(s)Stop server" << endl;
		cin >> operation;

		switch (operation)
		{
		case '1':
			try
			{
				registration();
			}
			catch (const exception& e)
			{
				cout << e.what() << endl;
			}
			break;
		case '2':
			login();
			break;
		case 'q':
			isChatWork_ = false;
			break;
		case 's':
			runServer_ = false;
			isChatWork_ = false;
			break;
		default:
			cout << "1 or 2" << endl;
			break;
		}
	} while (!currentUser_ && isChatWork_);
}

void Chat::showUserMenu()		//меню пользователя
{
	char operation;
	cout << "Hi, " << currentUser_->getUserName() << endl;

	while (currentUser_)
	{
		if (currentUser_-> getAdmin())			//проверка на админа
			cout << "Menu: (1) Show chat | (2) Add message | (3) Users |(d) Dell message | (q) Logout" << endl;
		else
			cout << "Menu: (1) Show chat | (2) Add message | (3) Users | (q) Logout" << endl;

		cin >> operation;

		switch (operation)
		{
		case '1':
			showChat();
			break;
		case '2':
			addMessage(this->cltptr);
			break;
		case '3':
			showAllUsersName();
			break;
		case 'd':
			dellMessage();
			break;
		case 'q':
			currentUser_ = nullptr;
			break;
		default:
			cout << "unknown choice" << endl;
			break;
		}
	}
}

void Chat::showAllUsersName() 		//показать всех пользователей
{
	string sendServ = "U%";
	cltptr->writeData(sendServ);
	messFromServ = cltptr->readData();

	if (messFromServ == "empty")
	{
        cout << "empty" << endl;
    }
    else 
	{
		transformMess(messFromServ);
		cout << "All Users: " << endl;
		for (int i = 0; i < fromServ.size() ; i += 2) {
			cout << setw(4) << left << fromServ[i];
            cout << setw(7) << left << fromServ[i+1] <<"\n";
		}
		cout << endl;
	}
}

void Chat::addMessage(shared_ptr<Client> clptr)			//добавить сообщение
{
	string from, to, text, messageSend;
	
	from = currentUser_->getUserName();

	cout << "To (name or all): " << endl;
	cin >> to;

	text = fullMess();		//функция с автозаполнением

	if (to == "all" || checkName(to))	
	{	
		Message m = Message(from, to, text);	
		messages_.push_back(m);
		messageSend = "M%" + from + '#' + to + '#' + text;
		clptr->writeData(messageSend);
		cltptr->readData();
	}
	else
	{
		cout << "error send message: cann't find " << to << endl;
		return;
	}
}

bool Chat::checkName(string& name)
{
	string sendServ="C%"+ name;
	cltptr->writeData(sendServ);
	if(cltptr->readData() == "true");
		return true;
	return false;
}

void Chat::transformMess(string& message)
{
    fromServ.clear();
    int start{0}, stop{0};

    start = message.find("%")+1;
    stop = message.find("#");
    while (start != 0)
    {
        fromServ.push_back(message.substr(start, stop - start));
        start = stop + 1;
        stop = message.find("#", start);
    }
}

 void Chat::dellMessage()				//удаление сообщения
 {
	string send ="D%";
	int numMes{};
	cout << "Enter the number of the message to delete: ";
	cin >> numMes;

	if(cin.fail())					
		cin.clear();

	send += to_string(numMes);
	cltptr->writeData(send);
	send = cltptr->readData();
	cout << send << endl;
	
}