//Name: Praveen Balireddy
//Roll: 2018201052

#ifndef CLIENT_H
#define CLIENT_H

#include <iostream>
#include <string>
#include <map>
#include "seeder.h"

class ClientHandler
{
    std::string ip;
    std::string port;
    std::map<std::string, std::string> filesLoc_dict;
    std::vector<seeder_Sptr> trackers;
    void createMtorrent(std::string path);
};

#endif