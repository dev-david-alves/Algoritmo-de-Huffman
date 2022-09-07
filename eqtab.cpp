#include "eqtab.h"

void eqtab::add(string s, char key) {
    tab.push_back(make_pair(s, key));
}

char eqtab::getChar(string s) {
    for (auto it = tab.begin(); it != tab.end(); it++) {
        if (it->first == s) {
            return it->second;
        }
    }
    
    return '\0';
}

string eqtab::getStr(char key) {
    for (auto it = tab.begin(); it != tab.end(); it++) {
        if (it->second == key) {
            return it->first;
        }
    }

    return "";
}