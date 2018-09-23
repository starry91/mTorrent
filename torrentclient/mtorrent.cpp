#include "mtorrent.h"
#include "utils.h"
#include <fstream>
#include <math.h>
#include "fileHandler.h"

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

void mTorrent::updateChunk(u_int32_t index, u_int32_t val)
{
    this->bit_chunks[index] = val;
}