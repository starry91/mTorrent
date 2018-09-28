#include "trackerDatabase.h"
#include <mutex>
#include <condition_variable>
#include <syslog.h>
#include <fstream>
#include "logHandler.h"
#include "TrackerServiceServer.h"
#include "errorMsg.h"

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
    LogHandler::getInstance().logMsg("Updated main tracker in Database");
}

void TrackerDatabase::setSecondaryTracker(Seeder tracker)
{
    this->tracker2 = tracker;
    LogHandler::getInstance().logMsg("Updated secondary tracker in Database");
}

void TrackerDatabase::setLogPath(std::string path)
{
    this->log_path = path;
    LogHandler::getInstance().logMsg("Updated log path in Database");
}

void TrackerDatabase::setSeederFilePath(std::string path)
{
    this->seeder_file_path = path;
    LogHandler::getInstance().logMsg("Updated seeder file path in Database");
}

void TrackerDatabase::addFileEntry(std::shared_ptr<FileAttr> file)
{
    std::lock_guard<std::mutex> lock(this->seeder_mtx);
    std::string hash = file->getHash();
    if (this->files.find(hash) != this->files.end())
    {
        //when only 1 seeder is there while sharing
        this->files[hash]->addSeeder(file->getSeeds()[0]);
    }
    else
    {
        this->files[hash] = file;
        LogHandler::getInstance().logMsg("Database: Added new file entry");
    }
}

void TrackerDatabase::addSeeder(std::string hash, seeder_Sptr seeder)
{
    std::lock_guard<std::mutex> lock(this->seeder_mtx);
    if (this->files.find(hash) != this->files.end())
    {
        this->files[hash]->addSeeder(seeder);
    }
    else
    {
        std::cout << "fail2" << std::endl;
        LogHandler::getInstance().logError("Database: No hash found for add seeder request for: " + hash);
        throw std::string("No hash found");
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
                LogHandler::getInstance().logMsg("Database: Removed seeder entry");
                this->files.erase(hash);
            }
        }
        else
        {
            LogHandler::getInstance().logError("Database: No hash found for add seeder request for: " + hash);
            throw std::string("No hash found");
        }
    }
}

std::vector<seeder_Sptr> TrackerDatabase::getSeederList(std::string hash)
{
    if (this->files.find(hash) != this->files.end())
    {
        return this->files[hash]->getSeeds();
        LogHandler::getInstance().logMsg("Database: fetching seeder list for hash: " + hash);
    }
    else
    {
        LogHandler::getInstance().logError("Database: No hash found for get seeder request for: " + hash);
        throw std::string("Error");
    }
}

Seeder TrackerDatabase::getMainTracker()
{
    return this->tracker1;
}

Seeder TrackerDatabase::getSecondayTracker()
{
    return this->tracker2;
}

void TrackerDatabase::readSeederfile()
{
    LogHandler::getInstance().logMsg("Database: Reading seeder file");
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

void TrackerDatabase::updateSeederfile()
{
    LogHandler::getInstance().logMsg("Database: Updating seeder file");
    std::ofstream out(this->seeder_file_path, std::ofstream::trunc);
    if (out.is_open())
    {
        for (auto i : this->files)
        {
            out << i.second->getFileName() << std::endl;
            out << i.second->getHash() << std::endl;
            for (auto seed : i.second->getSeeds())
            {
                out << seed->getIp() << ":" << seed->getPort() << std::endl;
            }
            out << std::endl;
        }
    }
}

std::string TrackerDatabase::getSeederFilePath()
{
    return this->seeder_file_path;
}

void TrackerDatabase::syncSeederFile()
{
    try
    {
        std::cout << "handleCommand() sending msg" << std::endl;
        TrackerServiceServer trackerCommunicator(TrackerDatabase::getInstance().getSecondayTracker());
        SyncSeederListRequest msg;
        auto res = trackerCommunicator.syncSeederFile(msg);

        this->writeToSeederfile(res.getData());
        //this->printResponse(msg.getType(), res);
        std::cout << "handleCommand() Got reponse" << std::endl;
    }
    catch (ErrorMsg e)
    {
    }
}

void TrackerDatabase::writeToSeederfile(std::vector<char> bytes)
{
    LogHandler::getInstance().logMsg("Database: Updating seeder file");
    std::ofstream out(this->seeder_file_path, std::ios::binary);
    if (out.is_open())
    {
        out.write(bytes.data(), bytes.size());
    }
    out.close();
}