#include "mtorrent.h"

mTorrent::mTorrent(std::string path, std::string hash, std::string name, int file_size, std::string bit_chunks)
{
    this->path = path;
    this->hash = hash;
    this->file_name = name;
    this->file_size = file_size;
    this->bit_chunks = bit_chunks;
}

std::string mTorrent::getfileName()
{
    return this->file_name;
}

std::string mTorrent::getBitChunks()
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