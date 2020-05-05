//
// Created by halim on 6.9.19..
//

#ifndef FSYSTEM_MEMORY_H
#define FSYSTEM_MEMORY_H


class Memory {
     int capacity;   //160 000 000 bita

public:
    Memory()
    {
        capacity=160000000;
    }
    void reduce(int n)
    {
        capacity+=n;
    }

    int getCapacity()
    {
        return capacity;
    }
};



#endif //FSYSTEM_MEMORY_H
