#pragma once
#include <string>
#include <vector>

#define ALPHABET_SIZE 26

struct TrieNode         //префиксное дерево
{
    struct TrieNode* children[ALPHABET_SIZE];
    bool isEndOfWord;
    int count;
};

TrieNode *getNewNode(void);                                 //создать дерево
void insert(TrieNode* root, std::string key);               //вставка по ключу
bool search(struct TrieNode *root, std::string key);        //поиск по ключу
bool isEmpty(TrieNode* root);                               //проверка на пустоту
TrieNode* remove(TrieNode* root, std::string key, int depth);       //удаление значения    
void findMinPrefixes(TrieNode* root, char buf[], int ind, std::string& res);    //кротчайшие префиксы
std::vector<std::string>& findWords(TrieNode* root, std::string prefix, std::vector<std::string>& vtr); //найти все слова по префиксу
int printAutoFillWords(TrieNode* root, std::string& prefx);     //вывод всех слов по префиксу