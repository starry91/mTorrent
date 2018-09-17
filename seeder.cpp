#include "seeder.h"

Seeder::Seeder(std::string ip, std::string port)
{
    this->ip = ip;
    this->port = port;
}

Seeder::Seeder(std::string ip_port)
{
    int index = ip_port.find(":");
    this->ip = ip_port.substr(0, index);
    this->port = ip_port.substr(index + 1, ip_port.length() - index);
}

std::string Seeder::getIp()
{
    return this->ip;
}
std::string Seeder::getPort()
{
    return this->port;
}

Seeder::Seeder()
{
}