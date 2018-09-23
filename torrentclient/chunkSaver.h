//Name: Praveen Balireddy
//Roll: 2018201052

#ifndef CHUNKSAVER_H
#define CHUNKSAVER_H

#include <string>
#include <iostream>
#include <map>
#include "seeder.h"
#include "mtorrent.h"
#include <fstream>
#include "TrackerServiceServer.h"

class ChunkSaver
{
    std::string filepath;
    std::vector<Seeder> seederlist;
    int chunkindex;
    std::string hash;

  public:
    ChunkSaver(std::string filepath, std::string hash, std::vector<Seeder> seeds, int chunkindex);
    void downloadChunk();
};

#endif