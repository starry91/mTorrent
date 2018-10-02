#include "downloadManager.h"
#include "TrackerServiceServer.h"
#include "clientDatabase.h"
#include "message.h"
#include "commandHandler.h"
#include <iomanip>
#include "errorMsg.h"
DownloadManager::DownloadManager() {}

DownloadManager &DownloadManager::getInstance()
{
    static DownloadManager dm;
    return dm;
}

void DownloadManager::addFile(down_Sptr dptr)
{
    this->dMap[dptr->getHash()] = dptr;
}

down_Sptr getFile(std::string hash)
{
}
void DownloadManager::updateFileChunkStatus(std::string hash, int index, int val)
{
    down_Sptr dptr;
    if (this->dMap.find(hash) != this->dMap.end())
        dptr = this->dMap[hash];
    else
        throw ErrorMsg("Cannot find chunk map in the download manager");

    dptr->updateChunkStatus(index, val);
    if (val == 1)
    {
        // std::cout << "In Download manager, Updating chunk info: " << dptr->getFileName() << " for chunk index: " << index << std::endl;
        dptr->incrementDownloadedChunks(); //setTotalChunks(dptr->getTotalChunks() + 1);
    }
}

void DownloadManager::addSeederRequestToTracker(std::string hash)
{
    TrackerServiceServer trackerCommunicator(ClientDatabase::getInstance().getTracker1(), ClientDatabase::getInstance().getTracker2());
    AddSeeder req;
    req.setHash(hash);
    req.setIp(ClientDatabase::getInstance().getHost().getIp());
    req.setPort(ClientDatabase::getInstance().getHost().getPort());
    Response res = trackerCommunicator.addSeederRequest(req);
}

void DownloadManager::printDownloads()
{
    std::cout << std::setw(15) << "File Name" << std::setw(10) << "Status" <<  std::setw(10) << "Progress" << std::endl;
    for (auto it : this->dMap)
    {
        auto file = it.second;
        std::cout << std::setw(15) << file->getFileName() << std::setw(10) 
        << (file->getStatus() == 1 ? "[S]" : "[D]") << std::setw(9) << (file->getDownloadedChunks()*100.00)/file->getTotalChunks() << "%" << std::endl;
    }
}

int DownloadManager::getDownloadStatus(std::string hash)
{
    return this->dMap[hash]->getStatus();
}

bool DownloadManager::hasDownloaded(std::string hash)
{
    if (this->dMap.find(hash) == this->dMap.end())
        return false;
    else
        return true;
}