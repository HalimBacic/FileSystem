//
// Created by halim on 6.9.19..
//

#ifndef FSYSTEM_SUPERBLOK_H
#define FSYSTEM_SUPERBLOK_H

#include <iostream>

using namespace std;
// Ukupno 20MB

class Superblok {
public:
    int size=160000000; // = 20MB
    int NUM_OF_NODS=9756; //1 inode na 16386b
    int BLOCK_SIZE=512000; //Veličina datoteke

    void print()
    {
        cout<<"Size:"<<this->size<<"kb"<<"\n";
        cout<<"BlockSize:"<<this->BLOCK_SIZE<<"kb"<<"\n";
        cout<<"NumOfInodes:"<<this->NUM_OF_NODS<<"\n";
    }
};


#endif //FSYSTEM_SUPERBLOK_H
