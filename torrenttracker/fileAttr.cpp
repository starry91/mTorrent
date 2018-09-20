#include "fileAttr.h"
#include <syslog.h>
FileAttr::FileAttr(std::string file_name, std::string hash, std::shared_ptr<Seeder> seed)
{
    this->file_name = file_name;
    this->hash = hash;
    this->seeds.push_back(seed);
}

FileAttr::FileAttr(std::string file_name, std::string hash, std::vector<seeder_Sptr> seeds)
{
    this->file_name = file_name;
    this->hash = hash;
    this->seeds = seeds;
}

void FileAttr::addSeeder(seeder_Sptr seeder)
{
    this->seeds.push_back(seeder);
}

void FileAttr::removeSeeder(seeder_Sptr seeder)
{
    syslog(0, "Removing file");
    syslog(0, "ip: %s", seeder->getIp().c_str());
    syslog(0, "port: %s", seeder->getPort().c_str());
    for (int i = 0; i < this->seeds.size(); i++)
    {
        if (seeds[i]->getIp() == seeder->getIp() && seeds[i]->getPort() == seeder->getPort())
        {
            syslog(0, "removing seed");
            seeds.erase(seeds.begin() + i);
        }
    }
}

int FileAttr::getSeederCount()
{
    return this->seeds.size();
}
std::string FileAttr::getFileName()
{
    return this->file_name;
}

std::vector<seeder_Sptr> FileAttr::getSeeds()
{
    return this->seeds;
}