#ifndef MTORRENT_H
#define MTORRENT_H

#include <iostream>
#include <string>
#include <map>
#include "chunk.h"
#include <vector>

class mTorrent
{
    std::string path;
    std::string hash;
    std::string file_name;
    int file_size;
    std::string bit_chunks;

  public:
    mTorrent(std::string path, std::string hash, std::string name, int file_size, std::string bit_chunks);
    std::string getPath();
    std::string getHash();
    std::string getfileName();
    int getFileSize();
    std::string getBitChunks();
};

typedef std::shared_ptr<mTorrent> mTorrent_Sptr;

#endif