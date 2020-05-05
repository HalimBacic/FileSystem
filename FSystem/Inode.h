//
// Created by halim on 6.9.19..
//

#ifndef FSYSTEM_INODE_H
#define FSYSTEM_INODE_H
#include <string>
#include <iostream>
#include <utility>
#include <array>
#include "Memory.h"
using namespace std;

class Inode {
    int flag; // Flag=1 inode zauzet, Flag=0 inode slobodan
    int file_size;
    string owner;
    array<pair<int,int>,7> adr;  //Niz parova koji sadrži početak i kraj zapisa u os
    string path;
    string name;
    bool type; //false = direktorijum true = datoteka
public:
    Inode();     //konstrukcija inodea
    void setName(std::string);    //seter za ime fajla na inodeu
    void setSize(int);    //seter za veličinu
    void setPath(std::string);    //seter za putanju fajla
    void setFlag(int);
    void setType(bool);
    void setAddr(int,int);    //Postavlja par start-end za fajl
    int getAddr();       //Vraća prvi slobodan član niza za upis novog bloka
    string getName();
    pair<int,int> getPair(int);
    string getPath();
    string getOwner();
    void getAddMem();
    bool getType();
    int getSize();
    int getFlag();
    void printInode();
    void resetInode();
    bool comparePath(std::string);
};


#endif //FSYSTEM_INODE_H
