#ifndef FILEHANDLER_H
#define FILEHANDLER_H

#include <iostream>
#include <string>
#include "mtorrent.h"
#define CHUNK_SIZE 524288

class FileHandler
{
  public:
    //generating mtorrent file
    void createMTorrent(mTorrent_Sptr torr);
    //getting the hash of the file
    std::string getFileHash(std::string file_name);
    long fileSize(std::string path);
    void readFileChunk(int chunk_index, std::string path, std::vector<char>& buf_b);
};

#endif
