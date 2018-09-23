#include "downloadManager.h"

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
        dptr->setTotalChunks(dptr->getTotalChunks() + 1);
    }
}