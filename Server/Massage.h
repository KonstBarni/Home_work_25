#pragma once
#include "User.h"

using namespace std;

class Message
{
	string _from, _to, _text;	//не константы потому что нужны для конструктора копирования и поератора присв-ия

public:
	Message(){_from = ""; _to = ""; _text = "";}
	Message(const string& from, const string& to, const string& text)
		: _from(from), _to(to), _text(text) {};

	Message& operator=(const Message& other)
    {
        if(&other != this)
        {
        	_from = other._from;
			_to = other._to;
			_text = other._text;
        }
        return *this;
    }

	Message(const Message &other)
	{
		_from = other._from;
		_to = other._to;
		_text = other._text;
	}

	const string& getFrom() const { return _from; }
	const string& getTo() const { return _to; }
	const string& getText() const { return _text; }
	const Message& getMessage(Message& mess) const { return mess;} 
	friend std::fstream& operator >>(std::fstream& is, Message& obj);
	friend std::ostream& operator <<(std::ostream& os, const Message& obj);
};