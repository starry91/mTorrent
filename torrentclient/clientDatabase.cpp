#include "clientDatabase.h"
#include "errorMsg.h"
ClientDatabase::ClientDatabase(){};

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
    std::lock_guard<std::mutex> lock(this->seeder_mtx);
    if (this->files.find(torr->getHash()) == this->files.end())
    {
        //std::cout << "In Client Database, adding new torfile: " << torr->getfileName() << std::endl;
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
    if (this->files.find(hash) != this->files.end())
    {
        return this->files[hash];
    }
    else
    {
        throw ErrorMsg("Could not find file entry for hash");
    }
}

bool ClientDatabase::hasFile(std::string hash)
{
    std::lock_guard<std::mutex> lock(this->seeder_mtx);
    if (this->files.find(hash) != this->files.end())
        return true;
    else
        return false;
}

void ClientDatabase::updateChunkInfo(std::string hash, int index, int val)
{
    std::lock_guard<std::mutex> lock(this->seeder_mtx);
    if (this->files.find(hash) != this->files.end())
    {
        this->files[hash]->updateChunk(index, val);
    }
}

std::mutex &ClientDatabase::getSeederMtx()
{
    return this->seeder_mtx;
}

void ClientDatabase::removeMTorrent(std::string hash)
{
    if (this->files.find(hash) != this->files.end())
    {
        //std::cout << "In client database, Removing file: " << this->files[hash]->getfileName() << std::endl;
        this->files.erase(hash);
    }
}