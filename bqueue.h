#include <iostream>
#include <string>
#include <queue> 

using namespace std;


class bqueue {
   queue<char> fila;
   FILE *fout;
public:
    bqueue(FILE* _file_);
    void pushString(string s);
};
