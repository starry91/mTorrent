#ifndef COMMANDHANDLER_H
#define COMMANDHANDLER_H

#include <iostream>
#include <string>

class CommandHandler {
    public:
    void handleCommand(std::string);
    void printResponse(Response);
};

#endif
