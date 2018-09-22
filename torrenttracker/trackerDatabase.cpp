#include "trackerDatabase.h"
#include <mutex>
#include <condition_variable>
#include <syslog.h>
#include <fstream>

TrackerDatabase::TrackerDatabase()
{
}

TrackerDatabase &TrackerDatabase::getInstance()
{
    static TrackerDatabase d1;
    return d1;
}

void TrackerDatabase::setMainTracker(Seeder tracker)
{
    this->tracker1 = tracker;
}

void TrackerDatabase::setSecondaryTracker(Seeder tracker)
{
    this->tracker2 = tracker;
}

void TrackerDatabase::setLogPath(std::string path)
{
    this->log_path = path;
}

void TrackerDatabase::setSeederFilePath(std::string path)
{
    this->seeder_file_path = path;
}

void TrackerDatabase::addFileEntry(std::shared_ptr<FileAttr> file)
{
    std::lock_guard<std::mutex> lock(this->seeder_mtx);
    std::string hash = file->getHash();
    if (this->files.find(hash) != this->files.end())
    {
        //when only 1 seeder is there while sharing
        this->files[hash]->addSeeder(file->getSeeds[0]);
    }
    else
    {
        this->files[hash] = file;
    }
}

void TrackerDatabase::addSeeder(std::string hash, seeder_Sptr seeder)
{
    std::lock_guard<std::mutex> lock(this->seeder_mtx);
    if (this->files.find(hash) != this->files.end())
    {
        this->files[hash]->addSeeder(seeder);
    }
}

void TrackerDatabase::remove_seeder(std::string hash, seeder_Sptr seeder)
{
    syslog(0, "seeder ip: %s", seeder->getIp().c_str());
    {
        std::lock_guard<std::mutex> lock(this->seeder_mtx);
        if (this->files.find(hash) != this->files.end())
        {
            auto &file = this->files[hash];
            syslog(0, "Seed Count: %d", file->getSeederCount());
            file->removeSeeder(seeder);
            syslog(0, "Seed Count: %d", file->getSeederCount());
            if (file->getSeederCount() == 0)
            {
                syslog(0, "Removing hash");
                this->files.erase(hash);
            }
        }
    }
}

std::vector<seeder_Sptr> TrackerDatabase::getSeederList(std::string hash)
{
    return this->files[hash]->getSeeds();
}

Seeder TrackerDatabase::getMainTracker()
{
    return this->tracker1;
}

void TrackerDatabase::readSeederfile()
{
    std::string file = this->seeder_file_path;
    std::string line;
    std::ifstream input(file);
    if (input.is_open())
    {
        bool eof = false;
        while (!eof)
        {
            std::string file_name, hash;
            std::vector<seeder_Sptr> seeds;
            if (std::getline(input, line))
            {
                file_name = std::string(line);
            }
            if (std::getline(input, line))
            {
                hash = std::string(line);
            }
            while (std::getline(input, line))
            {
                //syslog(0, "seeder: [%s]", line.c_str());
                if (std::string(line) != "")
                    seeds.push_back(std::make_shared<Seeder>(Seeder(std::string(line))));
                else
                    break;
            }
            if (seeds.size() != 0)
                this->files[hash] = std::make_shared<FileAttr>(FileAttr(file_name, hash, seeds));
            if (input.eof())
                eof = true;
            //syslog(0, "eof: %d", eof);
            //syslog(0, "Filename: %s Hash: %s, seeds: %d", file_name.c_str(), hash.c_str(), seeds.size());
            //syslog(0, "Seeder: %s", seeds[6]);
        }
        input.close();
    }
    for (auto it = this->files.begin(); it != this->files.end(); it++)
    {
        syslog(0, "read Hash: %s", it->first.c_str());
        auto file = it->second;
        auto seeds = file->getSeeds();
        for (auto it1 = seeds.begin(); it1 != seeds.end(); it1++)
        {
            syslog(0, "read ip: %s", (*it1)->getIp().c_str());
            syslog(0, "read port: %s", (*it1)->getPort().c_str());
        }
    }
}

bool TrackerDatabase::exists(std::string hash)
{
    if (this->files.find(hash) == this->files.end())
        return false;
    else
        return true;
}