#include <iostream>
#include <vector>
#include <string.h>
#include "seeder.h"
#include "trackerHandler.h"
#include <thread>
#include <sstream>
#include <syslog.h>

int main2(int argc, char *argv[])
{
    syslog(0, "--------------------------------------------------");
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

#include "message.h"

int main()
{
    Share s1;
    s1.setHash("xyz");
    s1.setIp("a.b.c.d");
    s1.setPort("1234");
    auto b = s1.getBytes();
    Share s2(b);
    std::cout << (s1.getIp() == s2.getIp()) << std::endl;
    std::cout << (s1.getPort() == s2.getPort()) << std::endl;
}