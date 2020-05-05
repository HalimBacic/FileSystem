//
// Created by halim on 10.9.19..
//

#include "Bitmap.h"

Bitmap::Bitmap() {
      for(int i=0;i<75;i++)
          this->inodes[i]=0;
}

void Bitmap::setInode(int i,int k) {
         this->inodes[i]=k;
}

int Bitmap::getInode() {
         int i=0;
         while(this->inodes[i]!=0)
             i++;
         return i;
}
