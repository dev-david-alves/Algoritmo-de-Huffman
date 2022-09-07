#pragma once

#include <iostream>
#include <list>
#include <string>

using namespace std;

class eqtab {
    list<pair<string, char>> tab;
public:
    void add(string s, char key);
    char getChar(string s);
    string getStr(char key);
};
