//Name: Praveen Balireddy
//Roll: 2018201052

#ifndef CHUNK_H
#define CHUNK_H

#include <string>
#include <iostream>

class Chunk {
    int begin;
    int offset;
    Chunk();
    Chunk(int begin, int offset);
    int getIndex();
};

#endif