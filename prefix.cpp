#include <iostream>

#include "prefix.h"

using namespace std;

void remove_node(prefix_node* root, prefix_node* node);

prefix_node* return_last_node(prefix_node* root);

prefix_node::prefix_node(unsigned char key, unsigned long freq) {
    this->c = key;
    this->freq = freq;
    this->prxox = nullptr;
    this->left = nullptr;
    this->right = nullptr;
}

prefix::prefix() {
    this->list_root = nullptr;
}

prefix_node* prefix::add_prefix(unsigned char key, unsigned long freq) {
    prefix_node *new_node = new prefix_node(key, freq);
    new_node->prxox = this->list_root;
    this->list_root = new_node;
    return new_node;
}

prefix_node* prefix::take_min_node() {
    prefix_node* min_node = list_root;
    prefix_node* aux = list_root;

    while(aux != nullptr) {
        if(aux->freq < min_node->freq) {
            min_node = aux;
        }
        aux = aux->prxox;
    }

    if(min_node == this->list_root) {
        this->list_root = list_root->prxox;
    } else {
        remove_node(this->list_root, min_node);
    }
    
    return min_node;
}

prefix_node* prefix::fusion(prefix_node *a, prefix_node *b) {
    prefix_node* new_node = new prefix_node('*', a->freq + b->freq);
    new_node->left = a;
    new_node->right = b;
    new_node->prxox = nullptr;

    if(this->list_root == nullptr) {
        this->list_root = new_node;
    } else {
        prefix_node* last_node = return_last_node(this->list_root);
        last_node->prxox = new_node;
    }

    return new_node;
}

void remove_node(prefix_node* root, prefix_node* node) {
    if(root == nullptr) {
        return;
    }

    while (root->prxox != nullptr) { 
        if(root->prxox == node) {
            root->prxox = node->prxox;
            node->prxox = nullptr;
            break;
        }

        root = root->prxox; 
    } 
}

prefix_node* return_last_node(prefix_node* root) {
    while(root->prxox != nullptr) {
        root = root->prxox;
    }

    return root;
}

prefix_node* prefix::get_root() {
    return this->list_root;
}