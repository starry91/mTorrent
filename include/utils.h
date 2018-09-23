
//Name: Praveen Balireddy
//Roll: 2018201052

#ifndef UTILS_H
#define UTILS_H


#include <string>
#include <iostream>
#include <memory>
#include <vector>
#include "mtorrent.h"
#include "seeder.h"
#define CHUNK_SIZE 524288

uint32_t nvtouint32(const std::vector<char> &arr);
std::vector<char> uint32tonv(uint32_t val);
std::vector<char> readBytes(int n, int sock_fd);
std::vector<std::string> extractArgs(std::string command);
//generating mtorrent file
void createMTorrent(mTorrent_Sptr torr);
//getting the hash of the file
std::string getFileHash(std::string file_name);
long fileSize(std::string path);
int createTCPClient(Seeder client);



#endif
