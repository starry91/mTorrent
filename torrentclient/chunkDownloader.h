//Name: Praveen Balireddy
//Roll: 2018201052

#ifndef CHUNKQUEUE_H
#define CHUNKQUEUE_H

#include <queue>
#include "chunkSaver.h"
#include <mutex>

class ChunkDownloader
{
    std::queue<ChunkSaver> q;
    std::mutex chunk_mtx;

  public:
    ChunkDownloader();
    bool isEmpty();
    ChunkSaver getNext();
    void add(ChunkSaver c);
    void downloadChunks();
};

#endif