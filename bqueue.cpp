#include "bqueue.h"
#include <math.h>

using namespace std;

bqueue::bqueue(FILE* _file_) {
    this->fout = _file_;
}

void bqueue::pushString(string s) {
    for(unsigned char c: s) {
        this->fila.push(c);

        if(this->fila.size() == 8) {
            int aux {0};
            int counter {0};

            while(!this->fila.empty()) {
                aux += pow(2, 7 - counter) * (this->fila.front() - '0');
                counter++;
                this->fila.pop();
            }

            unsigned char ch = (unsigned char) aux;
            fwrite(&ch, sizeof(unsigned char), 1, this->fout);
        }
    }
    fclose(this->fout);
}