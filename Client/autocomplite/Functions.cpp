#include <iostream>
#include <string>
#include <vector>
#include "Functions.h"
#include "Trie.h"
#include "Dictionary.h"

using namespace std;
string fullMessage;     //глобальная переменная для хранения сообщения

const string& fullMess()    //возвращает глобальное сообщение
{
    string lm;
    fullMessage.clear();
    TrieNode* dict = createDict();
    lm = inputMess(dict);
    fullMessage = lm;
    return fullMessage;
}

const string& choiceWord(vector<string> words)        //выбор слова из вектора
{
    int messIndex = 0;
    
    cin >> messIndex;
    if(cin.bad())
    {
        cin.clear();
        cout << "Its not a number!" << endl;
    }

    if(messIndex < 0 || messIndex > words.size())
    {
        cout << "Incorrect number of word!" << endl;
        cin.clear();
    }
    return words[messIndex];
}

const string& inputMess(TrieNode* dict)
{
    string message;     // конечное сообщение
    string inpWord;     // вводимое слово
    bool inpMessWork = true;    

    cout << "Use only 'a -z' lower case symbols (input '*' to show available words)" << endl;

    while(inpMessWork)      //пока вводится сообщение
    {
        cin >> inpWord;

        if (inpWord.empty())
            inpMessWork = false;

        if (inpWord.find('*') != std::string::npos)     //  условие запуска вывода автозаполнения
        {
            inpWord.pop_back();
            cout << "Choose the right word and press the corresponding number" << endl;
        
            int resOfFunc = printAutoFillWords(dict, inpWord);      //резельтат возврата функции

            if (resOfFunc == -1)        //если других слов нет 
            {
                cout << "No other strings found with this prefix" << endl;
                message += inpWord;
                inpWord.clear();
                cout << message;
            }
            else if (resOfFunc == 0)    //если слово по префиксу не найдено 
            {
                cout << "No string found with this prefix" << endl;
                inpWord.clear();
                cout << message;
            }

        //формирование вектор аслов для выбора слова для автозаполнения
            TrieNode* pNode = dict;
            for (char s : inpWord) 
            {
                int ind = ((int) s) - (int)'a';
                pNode = pNode->children[ind];
            }

            vector<string> vtr;
            vector<string> words = findWords(pNode, inpWord, vtr);
            inpWord.clear();
            inpWord = choiceWord(words);

            if(message.size()> 1)
                message += " ";

            message += inpWord;
            inpWord.clear();
            cout <<  message;
            continue;
        }
        else
        {
            if(message.size()> 1)
                message += " ";

            message += inpWord;
            inpWord.clear();
            inpMessWork = false;
        }
    }
    getline(cin, inpWord);
    message += inpWord;
    fullMessage += message;
    return fullMessage;
}