#pragma once
#include <fstream>
#include <string>

using namespace std;

class User
{
	string login_;
	string name_;
	string password_;
	bool admin_;

public:
	User();
	User(const string& login, const string& name, const string& password, bool admin)
		: login_(login), name_(name), password_(password), admin_(admin) {}

	const string& getUserLogin() const { return login_; }					//getters
	const string& getUserPassword() const { return password_; }
	const string& getUserName() const { return name_; }
	bool getAdmin() {return admin_;}

	void setUserPassword(const string& password) { password_ = password; }	//setters
	void setUserName(const string& name) { name_ = name; }
	friend std::istream& operator >>(std::istream& is, User& obj);
	friend std::ostream& operator <<(std::ostream& os, const User& obj);

};

