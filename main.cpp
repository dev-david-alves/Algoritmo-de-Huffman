#include <iostream>
#include <sstream>
#include <string>
#include <bitset>
#include <fstream>

#include "ftab.h"
#include "prefix.h"
#include "eqtab.h"
#include "bqueue.h"

using namespace std;

// Pega os códigos huffman do mais à esquerda para o mais à direita
void get_huffman_codes(prefix_node* root, string code, eqtab& eq) {
    if (root == nullptr) {
        return;
    }

    if (root->left == nullptr && root->right == nullptr) {
        eq.add(code, root->c);
        return;
    }

    code += '0';
    get_huffman_codes(root->left, code, eq);
    code.pop_back();

    code += '1';
    get_huffman_codes(root->right, code, eq);
    code.pop_back();
}

// Completa a string com zeros
string complete_string(string str, short int to_complete) {
    string zeros = string(to_complete, '0');

    return str + zeros;
}

// Lê o arquivo e retorna uma string com o conteúdo
string read_file(const char* filename) {
    ifstream file(filename, ios::binary);

    // Checa se o arquivo foi aberto com sucesso
    if (!file.is_open()) {
        cout << "Erro ao abrir o arquivo.\n";
        exit(1);
        return "";
    }

    stringstream buffer;

    buffer << file.rdbuf();

    return buffer.str();
}

// Monta a string com os códigos da tabela de equivalência
string mount_path(eqtab& eq, string str) {
    string codes = "";

    for(int i = 0; i < str.length(); i++) {
        codes += eq.getStr(str[i]);
    }

    return codes;
}

// Comprimir
void compress(const char* input_filename, const char* output_filename) {
    string text = read_file(input_filename);

    if(text.length() == 0) {
        cout << "Impossivel compactar, o arquivo esta vazio.\n";
        exit(1);
    }

    // Cria a tabela de frequência
    ftab f(input_filename);

    // Cria os nós da árvore de huffman
    prefix tree;
    int counter = 0;
    int count_nodes = 0;

    while(counter < 256) {
        unsigned char c = (unsigned char) counter;

        if(f[counter] > 0) {
            tree.add_prefix(c, f[c]);
            count_nodes++;
        }

        counter++;
    }

    if(count_nodes <= 1) {
        cout << "Impossivel compactar, arquivos com apenas um caractere, ou vazios, nao podem ser compactados.\n";
        exit(1);
    }

    // Cria a árvore de huffman
    while(tree.get_root()->prxox != nullptr) {
        prefix_node* a = tree.take_min_node();
        prefix_node* b = tree.take_min_node();
        
        tree.fusion(a, b);
    }

    eqtab eq;
    string eq_str = "";

    // Monta a tabela de equivalência
    get_huffman_codes(tree.get_root(), "", eq);

    eq_str = mount_path(eq, text);

    // Completa a string com zeros
    short int to_complete = 0;
    if(eq_str.length() % 8 != 0) {
        to_complete = 8 - (eq_str.length() % 8);
        eq_str = complete_string(eq_str, to_complete);
    }

    // Cria o arquivo de saída
    FILE* fout = fopen(output_filename, "wb");

    // Coloca a quantidade de caracteres a serem removidos no arquivo de saída como um short int (2 bytes)
    fwrite(&to_complete, sizeof(short int), 1, fout);

    // Coloca a tabela de frequência no arquivo compactado, ou seja, o arquivo de saída
    for(int i = 0; i < 256; i++) {
        unsigned char c = (unsigned char) i;
        unsigned long freq = (unsigned long) f[c];
        fwrite(&freq, sizeof(unsigned long), 1, fout);        
    }

    // Coloca a string de códigos no arquivo compactado, ou seja, o arquivo de saída
    bqueue bq(fout);
    bq.pushString(eq_str);
}

// Converte um unsigned char para uma string de 8 bits
string char_to_binary(unsigned char c) {
    stringstream ss;
    ss << bitset<8>(c);

    return ss.str();
}

// Percorre o binário, pega cada caractere e percorre a árvore, se for folha, adiciona o caractere achado na string
string get_text(prefix_node* list_root, prefix_node* root, string bin) {
    string text = "";

    for(int i = 0; i < bin.length(); i++) {
        if(bin[i] == '0') {
            root = root->left;
        } else {
            root = root->right;
        }

        if(root->left == nullptr && root->right == nullptr) {
            text += root->c;
            root = list_root;
        }
    }

    return text;
}

// Descomprimir
void decompress(const char* input_filename, const char* output_filename) {
    short int to_complete = 0;

    FILE* fin = fopen(input_filename, "rb");

    // Pega a quantidade de caracteres a serem removidos do arquivo compactado
    fread(&to_complete, sizeof(short int), 1, fin);

    // Pega a tabela de frequência do arquivo compactado
    unsigned long freq = 0;

    std::pair<unsigned char, unsigned long> tab[256];

    for(int i = 0; i < 256; i++) {
        fread(&freq, sizeof(unsigned long), 1, fin);
        tab[i] = std::make_pair((unsigned char) i, freq);
    }

    // Cria os nós da árvore de huffman
    prefix tree;
    int counter = 0;

    int count_nodes = 0;

    while(counter < 256) {
        unsigned char c = (unsigned char) counter;

        if(tab[counter].second > 0) {
            tree.add_prefix(c, tab[counter].second);
            count_nodes++;
        }

        counter++;
    }

    if(count_nodes <= 1) {
        cout << "Impossivel descomprimir, arquivos com apenas um caractere, ou vazios, nao podem ser descompactados.\n";
        exit(1);
    }

    // Cria a árvore de huffman
    while(tree.get_root()->prxox != nullptr) {
        prefix_node* a = tree.take_min_node();
        prefix_node* b = tree.take_min_node();
        
        tree.fusion(a, b);
    }

    string binary_str = "";

    unsigned char c {0};

    // Pega o binário do arquivo compactado
    while(fread(&c, sizeof(unsigned char), 1, fin)) {
        binary_str += char_to_binary(c);
    }

    // Remove os caracteres a serem removidos
    binary_str = binary_str.substr(0, binary_str.length() - to_complete);

    // Converte o binário para o texto passando pela árvore de huffman e pegando o caractere
    string decompressed_text = get_text(tree.get_root(), tree.get_root(), binary_str);

    // Salva o texto descomprimido no arquivo de saída
    ofstream output_file(output_filename, ios::binary);
    output_file << decompressed_text;
    output_file.close();
}
  
int main(int argc, char* argv[]) {
    // Você deve passar 3 argumentos para o programa
    // O "-c" ou "-d" indica se o programa deve comprimir ou descomprimir
    // O nome do arquivo de entrada
    // O nome do arquivo de saída

    if((string) argv[1] == "-c") {
        compress(argv[2], argv[3]);
    } else if((string) argv[1] == "-d") {
        decompress(argv[2], argv[3]);
    } else {
        cout << "Compactador de arquivos!\n";
        cout << "Necessario passar 3 argumentos para o programa -> (-c para compactar || -d para descompactar, o nome do arquivo de entrada e o nome do arquivo de saida).\n";
    }
  
    return 0;
}

// g++ main.cpp ftab.cpp prefix.cpp eqtab.cpp bqueue.cpp -o main && ./main -c texto.txt compactado.huf

// g++ main.cpp ftab.cpp prefix.cpp eqtab.cpp bqueue.cpp -o main && ./main -d compactado.huf descompactado.txt