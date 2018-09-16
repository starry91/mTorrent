#include "fileAttr.h"

FileAttr::FileAttr(std::string hash, std::vector<std::shared_ptr<Seeder>>)
{
}

void FileAttr::addSeeder(seeder_Sptr seeder)
{
    this->seeds.push_back(seeder);
}

void FileAttr::removeSeeder(seeder_Sptr seeder)
{
    for (int i = 0; i < this->seeds.size(); i++)
    {
        if (seeds[i]->getIp() == seeder->getIp() && seeds[i]->getPort() == seeder->getPort())
        {
            seeds.erase(seeds.begin() + i);
        }
    }
}

int FileAttr::getSeederCount()
{
    return this->seeds.size();
}
std::string FileAttr::getHash()
{
    return this->hash;
}