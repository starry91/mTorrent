//Name: Praveen Balireddy
//Roll: 2018201052

#ifndef CLIENT_H
#define CLIENT_H

#include <string>
#include <iostream>

class Client {
    int fd;
    public:
    std::string extractPayload();
    Client(int fd);
};

#endif