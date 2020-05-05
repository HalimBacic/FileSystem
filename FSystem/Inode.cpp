//
// Created by halim on 6.9.19..
//

#include "Inode.h"

Inode::Inode() {
    this->flag=0;
    this->owner="System";
    this->path="./";
    this->name="Unknown";
    this->file_size=0;

    for(int i=0;i<7;i++)
    { this->adr[i].first=0; this->adr[i].second=0; }
    this->type=false;
}

void Inode::setName(std::string name) {
   this->name=name;
}

void Inode::setSize(int n) {
  this->file_size=n;
}

void Inode::setPath(std::string path) {
    this->path=path;
}

void Inode::setFlag(int n) {
   this->flag=n;
}

string Inode::getName() {
    return this->name;
}

string Inode::getPath() {
    return this->path;
}

string Inode::getOwner() {
    return this->owner;
}

int Inode::getSize() {
    return this->file_size;
}

int Inode::getFlag() {
    return flag;
}

void Inode::printInode() {
     std::cout<<this->getFlag()<<"  "<<this->getName()<<"  "<<this->getPath()<<"  ";
     std::cout<<this->getOwner()<<"  "<<this->getSize()<<"  "<<"ADDR:";
     this->getAddMem();
     std::cout<<"\n";
}

bool Inode::comparePath(std::string upath) {   //TODO Provjeriti da li je je "last file"
       std::string check = this->getPath();
       int i;
       for(i=check.length(); check[i]!='/';--i);
       check.erase(i,this->getPath().length());
       return
           check == upath;
}

void Inode::setType(bool t) {
       this->type=t;
}

bool Inode::getType() {
    return this->type;
}

void Inode::resetInode() {
     this->setFlag(0);
     this->setName("Unknown");
     this->setSize(0);
     this->setPath("./");
     this->setType("false");
    for(int i=0;i<7;i++)
    { this->adr[i].first=0; this->adr[i].second=0; }
}

int Inode::getAddr() {
    int i=0;
    while(this->adr[i].first!=0) i++;
    return i;
}

void Inode::setAddr(int start,int end) {
    int i=this->getAddr();
    this->adr[i].first=start;
    this->adr[i].second=start+end;
}

pair<int, int> Inode::getPair(int i) {
    return this->adr[i];
}

void Inode::getAddMem() {
     int i=0;
     while(this->adr[i].first) {
         cout << this->adr[i].first << " ";
         i++;
     }
}

