//Name: Praveen Balireddy
//Roll: 2018201052

#ifndef CHUNK_H
#define CHUNK_H

#include <string>
#include <iostream>
#include <memory>
class Chunk
{
    int begin;
    int offset;
    Chunk();
    Chunk(int begin, int offset);
    int getIndex();
};

typedef std::shared_ptr<Chunk> chunk_Sptr;

#endif