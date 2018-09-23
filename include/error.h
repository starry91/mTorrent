#ifndef ERROR_H
#define ERROR_H

#include <string>
#include <iostream>

//Error class to handle errors

class Error
{
    std::string msg;
    public:
    Error(std::string msg);
    std::string getErrorMsg();
};
#endif