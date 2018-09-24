#include "downloadManager.h"
#include "TrackerServiceServer.h"
#include "clientDatabase.h"
#include "message.h"
#include "commandHandler.h"

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

    auto dptr = this->dMap[hash];
    dptr->updateChunkStatus(index, val);
    if (val == 1)
    {
       // std::cout << "In Download manager, Updating chunk info: " << dptr->getFileName() << " for chunk index: " << index << std::endl;
        dptr->incrementDownloadedChunks();//setTotalChunks(dptr->getTotalChunks() + 1);
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