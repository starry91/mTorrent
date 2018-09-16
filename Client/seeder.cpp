#include "seeder.h"

Seeder::Seeder(std::string ip, std::string port)
{
    this->ip = ip;
    this->port = port;
}

std::string Seeder::getIp()
{
    return this->ip;
}
std::string Seeder::getPort()
{
    return this->port;
}