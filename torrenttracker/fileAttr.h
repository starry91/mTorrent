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
  std::string file_name;
  std::string hash;
  std::vector<seeder_Sptr> seeds;

public:
  FileAttr(std::string file_name, std::string hash, std::shared_ptr<Seeder>);
  FileAttr(std::string file_name, std::string hash, std::vector<seeder_Sptr>);
  void addSeeder(seeder_Sptr);
  void removeSeeder(seeder_Sptr);
  int getSeederCount();
  std::vector<seeder_Sptr> getSeeds();
  std::string getFileName();
};

#endif