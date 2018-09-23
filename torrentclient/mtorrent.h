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
    std::vector<int> bit_chunks;

  public:
    mTorrent(std::string path, std::string name);
    std::string getPath();
    std::string getHash();
    std::string getfileName();
    int getFileSize();
    std::vector<int> getBitChunks();
    void updateChunk(int index, int val);
};

typedef std::shared_ptr<mTorrent> mTorrent_Sptr;

#endif