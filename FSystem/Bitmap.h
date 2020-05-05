//
// Created by halim on 10.9.19..
//

#ifndef FSYSTEM_BITMAP_H
#define FSYSTEM_BITMAP_H


class Bitmap {
        int inodes[9756];
public:
        Bitmap();
        void setInode(int i,int);
        int getInode();
};


#endif //FSYSTEM_BITMAP_H
