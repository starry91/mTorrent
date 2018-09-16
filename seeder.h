//Name: Praveen Balireddy
//Roll: 2018201052

#ifndef SEEDER_H
#define SEEDER_H

#include <string>
#include <iostream>
#include "chunk.h"
#include <vector>
#include <memory>

class Seeder {
    std::string ip;
    std::string port;
    public:
    Seeder(std::string ip, std::string port);
    std::string getIp();
    std::string getPort();
    //std::string file_path;
    //std::vector<std::shared_ptr<Chunk>> Chunks;
};

typedef std::shared_ptr<Seeder> seeder_Sptr;

#endif