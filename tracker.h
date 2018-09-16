//Name: Praveen Balireddy
//Roll: 2018201052

#ifndef TRACKER_H
#define TRACKER_H

#include <string>
#include <iostream>
#include <map>
#include "fileAttr.h"

class Tracker {
    std::map<std::string,std::shared_ptr<FileAttr>> files;
    public:
    void startListening(std::string ip, int port);
    void add_file(std::string, std::shared_ptr<FileAttr>);
    void add_file(int fd);
    void add_seeder(int fd);
    void remove_seeder(int fd);
    void remove_seeder (seeder_Sptr);
    void remove_file(std::string);
    void serviceRequest(int client_fd);
    std::string extractPayload(int fd);
};

typedef std::shared_ptr<FileAttr> fileAttr_Sptr;

#endif