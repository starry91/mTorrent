//Name: Praveen Balireddy
//Roll: 2018201052

#ifndef CLIENT_H
#define CLIENT_H

#include <string>
#include <iostream>
#include "seeder.h"

class Client {
    int fd;
    public:
    std::string extractPayload();
    void sendSeederData(std::vector<seeder_Sptr> seeds);
    Client(int fd);
};

#endif