//Name: Praveen Balireddy
//Roll: 2018201052

#ifndef TRACKERHANDLER_H
#define TRACKERHANDLER_H

#include <iostream>
#include <string>
#include <map>
#include "fileAttr.h"
#include "seeder.h"

class TrackerHandler
{
    Seeder tracker1;
    Seeder tracker2;
    std::string log_path;
    std::map<std::string, std::shared_ptr<FileAttr>> files;

  public:
    TrackerHandler(Seeder tracker1, Seeder tracker2);
    void startListening();
    void add_file(std::string, std::shared_ptr<FileAttr>);
    void add_file(int fd);
    void add_seeder(int fd);
    void remove_seeder(int fd);
    void remove_seeder(seeder_Sptr);
    void remove_file(std::string);
    void sendSeedList(int fd);
    void serviceRequest(int client_fd);
    void readSeederfile(std::string);
    void setLogPath(std::string path);
};

typedef std::shared_ptr<FileAttr> fileAttr_Sptr;

#endif