//Name: Praveen Balireddy
//Roll: 2018201052

#ifndef MSGHANDLER_H
#define MSGHANDLER_H

#include <string>
#include <iostream>
#include <memory>
#include <vector>
#include "message.h"

class ShareHandler {
    public:
    void HandleShareRequest(Message m, int sock_fd);
    void HandleShareResponse(Message m, int sock_fd);
};

#endif
