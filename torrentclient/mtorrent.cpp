#include "mtorrent.h"
#include "utils.h"
#include <fstream>
#include <math.h>

mTorrent::mTorrent(std::string path, std::string name)
{
    this->path = path;
    this->hash = getFileHash(path);
    this->file_name = name;
    this->file_size = fileSize(path);
    this->bit_chunks = std::vector<int>(ceil((file_size * 1.0000) / CHUNK_SIZE),1);
}

std::string mTorrent::getfileName()
{
    return this->file_name;
}

std::vector<int> mTorrent::getBitChunks()
{
    return this->bit_chunks;
}

int mTorrent::getFileSize()
{
    return this->file_size;
}

std::string mTorrent::getHash()
{
    return this->hash;
}

std::string mTorrent::getPath()
{
    return this->path;
}

void mTorrent::updateChunk(int index, int val) {
    this->bit_chunks[index] = val;
}