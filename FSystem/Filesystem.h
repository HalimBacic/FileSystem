//
// Created by halim on 7/22/19.
//

#ifndef FILESYSTEM_FILESYSTEM_H
#define FILESYSTEM_FILESYSTEM_H

#include "Superblok.h"
#include "Bitmap.h"
#include "Memory.h"
#include "Inode.h"
#include <experimental/filesystem>
#include <fstream>
#include <stdio.h>
#define NUM 9756

using namespace std;

class Filesystem {
    Bitmap bitmap;
    Superblok superblok;
    Inode inode[NUM];
    Memory memory;
    bool info;
    fstream file;
public:
    Filesystem();     //Konstruktor
    bool createRoot();   //kreiranje korijena
    bool createFoDir(std::string,std::string,bool);   //kreiranje direktorijuma ili datoteke
    void listingPath(std::string);       //metoda za opciju ls
    void renameName(std::string,std::string);    //metoda za rename
    bool echo(std::string,std::string);   //upis texta u datoteku
    void cat(std::string);    //ispis datoteke na konzolu
    void copy(std::string,std::string);  //kopiranje datoteke
    void move(std::string,std::string);   //pomjeranje datoteke
    void remove(std::string,std::string);
    void stat(std::string);
    void download(std::string,std::string);
    void upload(std::string);
    void find(std::string);
    void free();
    void help();
    static void deleteSystem();
};


#endif //FILESYSTEM_FILESYSTEM_H
