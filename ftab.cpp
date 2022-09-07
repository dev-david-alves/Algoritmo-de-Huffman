#include "ftab.h"

const int tab_size = 256;

ftab::ftab(const char* filename) {
    for (int i = 0; i < tab_size; i++) {
        this->tab[i].first = 0;
        this->tab[i].second = 0;
    }
    
    FILE* file = fopen(filename, "rb");

    mychar c{0};

    while(!feof(file)) {
        fread(&c, 1, 1, file);
        this->tab[c].first = c;
        this->tab[c].second++;
    }

    fclose(file);
    this->sort();
}

void ftab::sort() {
    for(int i = 0; i < tab_size; i++) {
        for(int j = 0; j < tab_size; j++) {
            if(this->tab[i].second < this->tab[j].second) {
                std::swap(this->tab[i], this->tab[j]);
            }
        }
    }
}

myint ftab::operator[](mychar k) {
    for(int i = 0; i < tab_size; i++) {
        if(this->tab[i].first == k) {
            return this->tab[i].second;
        }
    }

    return 0;
}

void ftab::print() {
    for(auto letter: this->tab) {
        if(letter.first != 0) {
            if(letter.first == ' ') {
                std::cout << "space bar" << " -> " << letter.second << "\n";
            } else {
                std::cout << letter.first << " -> " << letter.second << "\n";
            }
        }
    }
}