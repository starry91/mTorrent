#include "clientDatabase.h"

ClientDatabase &ClientDatabase::getInstance()
{
    static ClientDatabase res;
    return res;
}

void ClientDatabase::setTracker1(Seeder tracker)
{
    this->Tracker1 = tracker;
}

void ClientDatabase::setTracker2(Seeder tracker)
{
    this->Tracker2 = tracker;
}

void ClientDatabase::sethost(Seeder tracker)
{
    this->host = tracker;
}

void ClientDatabase::setLogPath(std::string path)
{
    this->log_path = path;
}

void ClientDatabase::addMTorrent(mTorrent_Sptr torr)
{
    if (this->files.find(torr->getHash()) == this->files.end())
    {
        this->files[torr->getHash()] = torr;
    }
}

Seeder ClientDatabase::getHost()
{
    return this->host;
}

Seeder ClientDatabase::getTracker1()
{
    return this->Tracker1;
}

Seeder ClientDatabase::getTracker2()
{
    return this->Tracker2;
}

mTorrent_Sptr ClientDatabase::getmTorrent(std::string hash)
{
    //handle error
    return this->files[hash];
}