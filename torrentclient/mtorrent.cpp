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
    std::cout << "In mTorrent class, Full file size: " << size << std::endl;
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

mTorrent::mTorrent(std::string path, std::string mTorr_path)
{

    auto filename = mTorr_path.substr(mTorr_path.find_last_of("/") + 1);
    std::cout << "In mTorrent, file name: " << filename << std::endl;
    ///put debugs here
    FileHandler filehandler;
    this->path = path;
    this->mTorr_path = mTorr_path;
    this->hash = filehandler.getFileHash(path);
    this->file_name = file_name;
    this->file_size = filehandler.fileSize(path);
    this->bit_chunks = std::vector<u_int32_t>(ceil((this->file_size * 1.0000) / CHUNK_SIZE), 1);
    std::cout << "In mtorr Class, " << this->bit_chunks.size() << std::endl;
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

std::string mTorrent::getMTorrPath()
{
    return this->mTorr_path;
}