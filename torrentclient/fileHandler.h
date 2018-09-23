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
};

#endif
