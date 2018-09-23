#ifndef LOGHANDLER_H
#define LOGHANDLER_H

//Logger class

class LogHandler {
    std::string log_path;
    LogHandler();
    public:
    static LogHandler& getInstance();
    void logMsg(std::string);
};

#endif