#include "Database.h"
#include <iostream>
#include <string>
#include "config.h"


bool Database::connectDatabase()
{
    mysql_init(&mysql);

    if (&mysql == nullptr) {
        cout << "Error: can't create MySQL-descriptor" << endl;
        exit(1);
    }

    if (!mysql_real_connect(&mysql, HOST, USER, PASS, DBNAME, 0, nullptr, 0)) {
        cout << "Error: can't connect to database " << mysql_error(&mysql) << endl;
        return false;
    }
    else {
        cout << "Success!" << endl;
    }

    return true;
}


void Database::disconnect() {
    mysql_close(&mysql);
}

void Database::checkLogin(vector<string>& fromClient) {
    string send;
    string query = "SELECT name, isadmin, password_hash FROM users"
        " JOIN passwords ON users.user_id = passwords.user_id"
        " WHERE login = '" + fromClient[0] + "'";  
    mysql_query(&mysql, query.c_str());
    res = mysql_store_result(&mysql);
    if (res->row_count != 0) {
        
        convertResToVector(res);
        send = "L%" + conversion[0] + "#" + conversion[1];
        mysql_free_result(res);
        this->setAnswer(send);
    }
    else
    {
        send = "error";
        this->setAnswer(send);
    }
}


bool Database::checkName(vector<string>& fromClient) {
    string getName = "SELECT name FROM users "
        "WHERE name = '" + fromClient[0] + "'";

    if (mysql_query(&mysql, getName.c_str()) == 0) {
        res = mysql_store_result(&mysql);
        if (res != nullptr) {
            mysql_free_result(res);
            return true;
        }
    }
    return false;
}


bool Database::checkRegister(vector<string>& fromServ) {
    string getLogin = "SELECT login, name FROM users "
        "WHERE login = '" + fromServ[0] + "' AND name = '" +fromServ[1]+"'";

    if (mysql_query(&mysql, getLogin.c_str()) == 0) {
        res = mysql_store_result(&mysql);
        if (res != nullptr) {
            mysql_free_result(res);
            return true;
        }
    }
    return false;
}


bool Database::registerUser(vector<string>& fromClient) {
    string send;
    string registerUserQuery = "INSERT INTO users (name, login, isadmin) "
        "VALUES ('" + fromClient[0] + "', '" + fromClient[1] + "'," +"'"+(fromClient[3][0]== 'f'? '0': '1')+"'" +")";

    string addUserPasswordQuery =
        "INSERT INTO passwords (user_id, password_hash) "
        "VALUES ((SELECT LAST_INSERT_ID()), '" + fromClient[2] + "')";

    if (mysql_query(&mysql, registerUserQuery.c_str()) == 0 &&
        mysql_query(&mysql, addUserPasswordQuery.c_str()) == 0) {
        send = "Register complite";
        this->setAnswer(send);
        return true;
    }
    send = "error";
    this->setAnswer(send);
    return false;
}

bool Database::addMessage(vector<string>& fromClient) {
    string send;
    string getFromId = "SELECT user_id FROM users WHERE name = '" + fromClient[0] + "'";
    mysql_query(&mysql, getFromId.c_str());
    res = mysql_store_result(&mysql);
    if (res!=nullptr) {
        if (row = mysql_fetch_row(res)) {
            int fromId = atoi(row[0]);
            mysql_free_result(res);
            
            string getToId = "SELECT user_id FROM users WHERE name = '" + fromClient[1] + "'";
            if (mysql_query(&mysql, getToId.c_str()) == 0) {
                res = mysql_store_result(&mysql);
                if (row = mysql_fetch_row(res)) {
                    int toId = atoi(row[0]);
                    mysql_free_result(res);

                    string addMsg =
                        "INSERT INTO messages (from_id, to_id, message) "
                        "VALUES ('" + to_string(fromId) + "', '" + to_string(toId) + "', '" + fromClient[2] + "')";
                    if (mysql_query(&mysql, addMsg.c_str()) == 0) {
                        send = "Message wos add";
                        this->setAnswer(send);
                        return true;
                    }
                }
            }
        }
    }
    send = "Message wos not add";
    this->setAnswer(send);

    return false;
}


void Database::getAllMessages(string& name) {

    string queryUsers = "SELECT message_id, u1.name AS from_name, "
        "u2.name AS to_name, m.message FROM messages m " 
        "JOIN users u1 ON u1.user_id = m.from_id "
        "JOIN users u2 ON u2.user_id = m.to_id "
        "WHERE(u1.name = '"+name+"' OR u2.name = '"+name+"') OR m.to_id = '1'";

    mysql_query(&mysql, queryUsers.c_str());
    res = mysql_store_result(&mysql);

    if (res != nullptr) 
    {
        convertResToVector(res);
        string send = "S%";
        vector<string>::iterator it = conversion.begin();

        for(int i{0}; i < conversion.size(); i++)
            send += conversion[i] + "#";

        this->setAnswer(send);
        mysql_free_result(res);
    }
    
}


void Database::getAllUsers() {
    string send = "U%";
    string getAllUsers = "SELECT user_id, name FROM users";

    if (mysql_query(&mysql, getAllUsers.c_str()) == 0) {
        res = mysql_store_result(&mysql);
        convertResToVector(res);
        mysql_free_result(res);

        for(int i{0}; i < conversion.size(); i++)
            send += conversion[i] + "#";
        this->setAnswer(send);
    }
}

void Database::delMessage(string num)
{
    string dellQuety = "DELETE FROM messages WHERE message_id = '"+ num +"'";

    if (mysql_query(&mysql, dellQuety.c_str()) == 0)
    {
        string send = "Delete wos complit";
        this->setAnswer(send);
    }else
    {
        string send = "Deletion faild ";
        this->setAnswer(send);
    }

}

void Database::convertResToVector(MYSQL_RES* res) {

    conversion.clear();
    while (row = mysql_fetch_row(res)) {
        for (int i = 0; i < mysql_num_fields(res); i++) {
            if (row[i] != nullptr) {
                conversion.push_back(string(row[i]));
            }
        }
    }
}