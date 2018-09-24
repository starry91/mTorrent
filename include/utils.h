
//Name: Praveen Balireddy
//Roll: 2018201052

#ifndef UTILS_H
#define UTILS_H


#include <string>
#include <iostream>
#include <memory>
#include <vector>
#include "seeder.h"

uint32_t nvtouint32(const std::vector<char> &arr);
std::vector<char> uint32tonv(uint32_t val);
std::vector<char> readBytes(int n, int sock_fd);
std::vector<std::string> extractArgs(std::string command);
int createTCPClient(Seeder client);
std::string getChunkHash(std::vector<char> bytes);

#endif
