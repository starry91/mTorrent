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
    std::vector<chunk_Sptr> chunks;
};

typedef std::shared_ptr<mTorrent> mTorrent_Sptr;

#endif