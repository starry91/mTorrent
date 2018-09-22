
//Name: Praveen Balireddy
//Roll: 2018201052

#ifndef UTILS_H
#define UTILS_H


#include <string>
#include <iostream>
#include <memory>
#include <vector>

uint32_t nvtouint32(const std::vector<char> &arr);

std::vector<char> uint32tonv(uint32_t val);

std::vector<char> readBytes(int n, int sock_fd);
#endif
