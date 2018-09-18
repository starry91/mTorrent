#include <iostream>
#include <vector>
#include <string.h>
#include "seeder.h"
#include "clientHandler.h"
#include <thread>
#include <sstream>
#include <syslog.h>
#include <memory>
#include <vector>

int main(int argc, char *argv[])
{
    syslog(0, "--------------------------------------------------");
    if (argc < 5)
    {
        std::cerr << "Not enough arguments" << std::endl;
        exit(2);
    }

    auto ip_host = std::string(strtok(argv[1], ":"));
    auto port_host = std::string(strtok(NULL, ":"));
    auto ip_1 = std::string(strtok(argv[2], ":"));
    auto port_1 = std::string(strtok(NULL, ":"));
    auto ip_2 = std::string(strtok(argv[3], ":"));
    auto port_2 = std::string(strtok(NULL, ":"));

    auto host = std::make_shared<Seeder>(Seeder(ip_1, port_1));
    auto tracker1 = std::make_shared<Seeder>(Seeder(ip_1, port_1));
    auto tracker2 = std::make_shared<Seeder>(Seeder(ip_2, port_2));

    //throws logic error
    // Seeder tracker1(std::string(strtok(argv[1], ":")), std::string(strtok(NULL, ":")));
    // Seeder tracker2(std::string(strtok(argv[2], ":")), std::string(strtok(NULL, ":")));

    std::cout << "hello" << std::endl;
    std::cout << "ip: " << tracker1->getIp() << " port: " << tracker1->getPort() << std::endl;
    std::cout << "ip: " << tracker2->getIp() << " port: " << tracker2->getPort() << std::endl;
    ClientHandler handler(host, tracker1, tracker2);
    while (true)
    {
        std::string command;
        std::getline(std::cin, command);
        handler.handleCommand(command);
    }
}