#include "networkInterfacer.h"
#include <unistd.h>
#include "utils.h"
#include <cstdint>
#include <cassert>

//-------------------------------------------------------------Network Reader
NetworkReader::NetworkReader(int sock_fd)
{
    this->client_fd = sock_fd;
}

NetworkReader::NetworkReader(){};

std::vector<char> NetworkReader::readFromNetwork(int sock_fd)
{
    auto magic_start_b = readBytes(11, sock_fd);

    if (std::string(magic_start_b.begin(), magic_start_b.end()) == magicStart)
    {
        auto header_b = readBytes(4, sock_fd);
        uint32_t payload_size = nvtouint32(header_b);
        auto payload = readBytes(payload_size, sock_fd);

        auto magic_start_e = readBytes(11, sock_fd);
        if (std::string(magic_start_e.begin(), magic_start_e.end()) == magicEnd)
        {
            return payload;
        }
    }
    else
    {
        std::vector<char> v;
        return v;
    }
}

std::vector<char> NetworkReader::readFromNetwork()
{
    auto magic_start_b = readBytes(11, this->client_fd);

    if (std::string(magic_start_b.begin(), magic_start_b.end()) == magicStart)
    {
        auto header_b = readBytes(4, this->client_fd);
        uint32_t payload_size = nvtouint32(header_b);
        auto payload = readBytes(payload_size, this->client_fd);

        auto magic_start_e = readBytes(11, this->client_fd);
        if (std::string(magic_start_e.begin(), magic_start_e.end()) == magicEnd)
        {
            return payload;
        }
    }
    else
    {
        std::vector<char> v;
        return v;
    }
}

//---------------------------------------------------------------Network Writer

NetworkWriter::NetworkWriter(){};
NetworkWriter::NetworkWriter(int sock_fd)
{
    this->client_fd = sock_fd;
}

void NetworkWriter::writeToNetwork(int sock_fd, std::vector<char> b)
{
    std::string magic_start = magicStart;
    write(sock_fd, magic_start.c_str(), magic_start.length());
    write(sock_fd, reinterpret_cast<char *>(b.data()), b.size());
    std::string magic_end = magicEnd;
    write(sock_fd, magic_end.c_str(), magic_end.length());
}

void NetworkWriter::writeToNetwork(std::vector<char> b)
{
    std::string magic_start = magicStart;
    write(this->client_fd, magic_start.c_str(), magic_start.length());
    write(this->client_fd, reinterpret_cast<char *>(b.data()), b.size());
    std::string magic_end = magicEnd;
    write(this->client_fd, magic_end.c_str(), magic_end.length());
}
