#include "client.h"
#include <sys/socket.h>
#include <unistd.h>
#include <vector>
#include <string>
#include <arpa/inet.h>
#include <syslog.h>

std::string Client::extractPayload()
{
    char size_buffer[1024];
    long valread;
    read(fd, &valread, sizeof(valread));
    //size_buffer[32] = 0;
    long byte_count = ntohl(valread);
    syslog(0, "Size: %d", byte_count);
    char payload[1024];
    read(fd, payload, byte_count);
    payload[byte_count] = '\0';
    syslog(0, "paylod: %s", payload);
    return std::string(payload);
}

Client::Client(int fd)
{
    this->fd = fd;
}

void Client::sendSeederData(std::vector<seeder_Sptr> seeds)
{
    //sending count of seeders
    u_int32_t size = htonl(seeds.size());
    send(this->fd, &size, sizeof(size), 0);
    for (int i = 0; i < seeds.size(); i++)
    {
        //sending ip
        auto ip = seeds[i]->getIp();
        u_int32_t size_ip = htonl(ip.length());
        send(this->fd, &size_ip, sizeof(size_ip), 0);
        send(this->fd, ip.c_str(), ip.length(), 0);

        //sending port
        auto port = seeds[i]->getPort();
        u_int32_t size_port = htonl(port.length());
        send(this->fd, &size_port, sizeof(size_port), 0);
        send(this->fd, port.c_str(), port.length(), 0);
    }
}