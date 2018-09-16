#include "chunk.h"

Chunk::Chunk(int begin, int offset)
{
    this->begin = begin;
    this->offset = offset;
}

int Chunk::getIndex() {
    return begin/512;
}