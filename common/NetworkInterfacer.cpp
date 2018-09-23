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

std::vector<char> NetworkReader::readFromNetwork()
{
    auto magic_start_b = readBytes(11, this->client_fd);

    if (std::string(magic_start_b.begin(), magic_start_b.end()) == magicStart)
    {
        auto header_b = readBytes(4, this->client_fd);
        uint32_t payload_size = nvtouint32(header_b);
        std::cout << "Payload size: " << payload_size << std::endl;
        auto payload = readBytes(payload_size, this->client_fd);

        auto magic_start_e = readBytes(11, this->client_fd);
        std::cout << "expected: " << std::string(magic_start_e.begin(), magic_start_e.end()) << " Actual: " << magicEnd << std::endl;
        if (std::string(magic_start_e.begin(), magic_start_e.end()) == magicEnd)
        {
            return payload;
        }
        else
        {
            std::cout << " Else in if" << std::endl;
            std::vector<char> v;
            return v;
        }
    }
    else
    {
        std::cout << " Else in network" << std::endl;
        std::vector<char> v;
        return v;
    }
}

//---------------------------------------------------------------Network Writer

NetworkWriter::NetworkWriter(int sock_fd)
{
    this->client_fd = sock_fd;
}

void NetworkWriter::writeToNetwork(std::vector<char> b)
{
    std::string magic_start = magicStart;
    write(this->client_fd, magic_start.c_str(), magic_start.length());
    auto payload_size = uint32tonv(b.size());
    write(this->client_fd, reinterpret_cast<char *>(payload_size.data()), payload_size.size());
    write(this->client_fd, reinterpret_cast<char *>(b.data()), b.size());
    std::string magic_end = magicEnd;
    write(this->client_fd, magic_end.c_str(), magic_end.length());
}
