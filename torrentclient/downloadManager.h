#ifndef DOWNLOADMANAGER_H
#define DOWNLOADMANAGER_H

#include <map>
#include "download.h"

class DownloadManager {
    std::map<std::string, down_Sptr> dMap;
    DownloadManager();
    public:
    static DownloadManager& getInstance();
    void addFile(down_Sptr);
    down_Sptr getFile(std::string hash);
    void updateFileChunkStatus(std::string hash, int index, int val);
    void addSeederRequestToTracker(std::string hash);
};

#endif
