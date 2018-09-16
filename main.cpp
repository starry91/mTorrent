#include <iostream>
#include <vector>
#include <string.h>
#include "seeder.h"
#include "trackerHandler.h"
#include <thread>
#include <sstream>

int main(int argc, char *argv[])
{
    if (argc < 5)
    {
        std::cerr << "Not enough arguments" << std::endl;
        exit(2);
    }

    auto ip_1 = std::string(strtok(argv[1], ":"));
    auto port_1 = std::string(strtok(NULL, ":"));
    auto ip_2 = std::string(strtok(argv[2], ":"));
    auto port_2 = std::string(strtok(NULL, ":"));
    //std::cout << "ip: " << tracker2.getIp() << " port: " << tracker2.getPort() << std::endl;
    Seeder tracker1(ip_1, port_1);
    Seeder tracker2(ip_2, port_2);

    //throws logic error
    // Seeder tracker1(std::string(strtok(argv[1], ":")), std::string(strtok(NULL, ":")));
    // Seeder tracker2(std::string(strtok(argv[2], ":")), std::string(strtok(NULL, ":")));

    std::cout << "hello" << std::endl;
    std::cout << "ip: " << tracker1.getIp() << " port: " << tracker1.getPort() << std::endl;
    std::cout << "ip: " << tracker2.getIp() << " port: " << tracker2.getPort() << std::endl;
    TrackerHandler tracker(tracker1, tracker2);
    tracker.readSeederfile(std::string(argv[3]));
    tracker.startListening();
}