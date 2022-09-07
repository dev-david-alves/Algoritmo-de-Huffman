#pragma once

#include <iostream>
#include <cstdio>

typedef unsigned char mychar;
typedef unsigned long long int myint;

using namespace std;

class ftab {
    std::pair<mychar, myint> tab[256];
    public:
        ftab(const char* filename);
        void sort();
        myint operator[](mychar k);
        void print();
};