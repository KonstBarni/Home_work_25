#include "User.h"

User::User()
{
	login_ = ""; name_ = ""; password_ = ""; admin_ = false;
}

std::istream& operator >>(std::istream& is, User& obj)
{
	is >> obj.login_;
	is >> obj.name_;
	is >> obj.password_;
	is >> obj.admin_;
	return is;
}

std::ostream& operator <<(std::ostream& os, const User& obj)
{
	os << obj.login_;
	os << ' ';
	os << obj.name_;
	os << ' ';
	os << obj.password_;
	os << ' ';
	os << obj.admin_;
	return os;
}