#ifndef CLIENTHANDLER_H
#define CLIENTHANDLER_H

#include <iostream>
#include <string>
#include <map>
#include "seeder.h"
#include "mtorrent.h"

class ClientHandler
{
    seeder_Sptr host;
    seeder_Sptr tracker1;
    seeder_Sptr tracker2;
    std::string log_path;
    std::map<std::string, mTorrent_Sptr> files;

  public:
    ClientHandler(seeder_Sptr, seeder_Sptr, seeder_Sptr);
    void createMTorrent(mTorrent_Sptr);
    std::vector<std::string> extractArgs(std::string);
    void shareFile(std::string file_name);
    void startListening();
    void serviceRequest(int client_fd);
    void handleCommand(std::string command);
    std::string getFileHash(std::string file);
    void addMTorrent(mTorrent_Sptr);
};

#endif