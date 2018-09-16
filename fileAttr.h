//Name: Praveen Balireddy
//Roll: 2018201052

#ifndef FILEATTR_H
#define FILEATTR_H

#include <string>
#include <iostream>
#include <vector>
#include "seeder.h"

class FileAttr
{
    std::string hash;
    //int file_size;
    std::vector<seeder_Sptr> seeds;

  public:
    FileAttr(std::string hash);
    FileAttr(std::string hash, std::vector<seeder_Sptr>);
    FileAttr(std::string hash, std::shared_ptr<Seeder>);
    void addSeeder(seeder_Sptr);
    void removeSeeder(seeder_Sptr);
    int getSeederCount();
    std::string getHash();
};

#endif