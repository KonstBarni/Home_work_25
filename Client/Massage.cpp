#include "Massage.h"

std::fstream& operator >>(std::fstream& is, Message& obj)
{
	is >> obj._from;
	is >> obj._to;
	is.ignore(1);
	std::getline(is.ignore(-1), obj._text);
	return is;
}

std::ostream& operator <<(std::ostream& os, const Message& obj)
{
	os << obj._from;
	os << ' ';
	os << obj._to;
	os << ' ';
	os << obj._text;
	return os;
}