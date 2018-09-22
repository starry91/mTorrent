#include "msgHandler.h"

void ShareHandler::HandleShareRequest(Message m, int fd)
{
    std::string file_name = client.extractPayload();
    std::string hash = client.extractPayload();
    std::string ip = client.extractPayload();
    std::string port = client.extractPayload();
    auto file_attr = std::make_shared<FileAttr>(FileAttr(file_name, hash, std::make_shared<Seeder>(Seeder(ip, port))));
    if (this->files.find(hash) != this->files.end())
    {
        //error
    }
    else
    {
        this->files[hash] = file_attr;
    }
}
