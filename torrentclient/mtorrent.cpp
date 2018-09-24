#include "mtorrent.h"
#include "utils.h"
#include <fstream>
#include <math.h>
#include "fileHandler.h"

mTorrent::mTorrent(){};

//setters
void mTorrent::setPath(std::string path)
{
    this->path = path;
}
void mTorrent::setHash(std::string hash)
{
    this->hash = hash;
}
void mTorrent::setFileName(std::string name)
{
    this->file_name = name;
}
void mTorrent::setFileSize(long size)
{
    this->file_size = size;
}
void mTorrent::setBitChunks(std::vector<u_int32_t> chunk_map)
{
    this->bit_chunks = chunk_map;
}

void mTorrent::clearBitChunks()
{
    std::fill(this->bit_chunks.begin(), this->bit_chunks.end(), 0);
}

mTorrent::mTorrent(std::string path, std::string name)
{
    FileHandler filehandler;
    this->path = path;
    this->hash = filehandler.getFileHash(path);
    this->file_name = name;
    this->file_size = filehandler.fileSize(path);
    this->bit_chunks = std::vector<u_int32_t>(ceil((this->file_size * 1.0000) / CHUNK_SIZE), 1);
}

std::string mTorrent::getfileName()
{
    return this->file_name;
}

std::vector<u_int32_t> mTorrent::getBitChunks()
{
    return this->bit_chunks;
}

long mTorrent::getFileSize()
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

void mTorrent::updateChunk(u_int32_t index, u_int32_t val)
{
    this->bit_chunks[index] = val;
}