#include "chunkDownloader.h"
#include "errorMsg.h"

ChunkDownloader::ChunkDownloader(){};

bool ChunkDownloader::isEmpty()
{
    return q.empty();
}

ChunkSaver ChunkDownloader::getNext()
{
    std::lock_guard<std::mutex> lck(this->chunk_mtx);
    if (this->isEmpty())
    {
        throw ErrorMsg("list empty");
    }
    else
    {
        ChunkSaver temp = q.front();
        q.pop();
        return temp;
    }
}

void ChunkDownloader::add(ChunkSaver c)
{
    std::lock_guard<std::mutex> lck(this->chunk_mtx);
    q.push(c);
}

void ChunkDownloader::downloadChunks()
{
    try
    {
        while (true)
        {
            auto temp = this->getNext();
            temp.downloadChunk();
        }
    }
    catch (ErrorMsg e)
    {
    }
}