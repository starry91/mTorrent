//Name: Praveen Balireddy
//Roll: 2018201052

#ifndef ClientDATABASE_H
#define ClientDATABASE_H

#include <string>
#include <iostream>
#include <map>
#include "fileAttr.h"
#include "seeder.h"
#include <mutex>
#include <condition_variable>
#include "mtorrent.h"
class ClientDatabase
{
  private:
    std::mutex seeder_mtx; // mutex for critical section
    Seeder Tracker1;
    Seeder Tracker2;
    Seeder host;
    std::string log_path;
    std::map<std::string, mTorrent_Sptr> files;
    ClientDatabase();

  public:
    static ClientDatabase &getInstance();
    void setTracker1(Seeder tracker);
    void setTracker2(Seeder tracker);
    void sethost(Seeder tracker);
    void setLogPath(std::string path);
    void addMTorrent(mTorrent_Sptr);
    
    Seeder getHost();
    Seeder getTracker1();
    Seeder getTracker2();
    mTorrent_Sptr getmTorrent(std::string hash);
};

#endif