#ifndef MTORRENT_H
#define MTORRENT_H

#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <memory>

class mTorrent
{
    std::string path;
    std::string hash;
    std::string file_name;
    long file_size;
    std::vector<u_int32_t> bit_chunks;

  public:
    mTorrent(std::string path, std::string name);
    std::string getPath();
    std::string getHash();
    std::string getfileName();
    int getFileSize();
    std::vector<u_int32_t> getBitChunks();
    void updateChunk(u_int32_t index, u_int32_t val);
};

typedef std::shared_ptr<mTorrent> mTorrent_Sptr;

#endif