#include <iostream>
#include <vector>
#include <string.h>
#include "seeder.h"
#include <thread>
#include <sstream>
#include <syslog.h>
#include <memory>
#include <vector>
#include <syslog.h>
#include <syslog.h>
#include "message.h"
#include "networkInterfacer.h"
#include "TrackerServiceServer.h"
#include "commandHandler.h"
#include "clientDatabase.h"
#include "logHandler.h"
#include "peerHandler.h"
#include "peerListener.h"

using std::cout;
using std::endl;

int main(int argc, char *argv[])
{
    syslog(0, "--------------------------------------------------");
    if (argc < 3)
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

    auto host = Seeder(ip_host, port_host);
    auto tracker1 = Seeder(ip_1, port_1);
    auto tracker2 = Seeder(ip_2, port_2);

    // std::cout << "tracker1 ip: " << tracker1.getIp() << "tracker1 port: " << tracker1.getPort() << std::endl;
    // std::cout << "tracker2 ip: " << tracker2.getIp() << "tracker2 port: " << tracker2.getPort() << std::endl;

    ClientDatabase::getInstance().sethost(host);
    ClientDatabase::getInstance().setTracker1(tracker1);
    ClientDatabase::getInstance().setTracker2(tracker2);

    if (argc == 5)
    {
        // cout << "got log path" << endl;
        ClientDatabase::getInstance().setLogPath(std::string(argv[4]));
        LogHandler::getInstance().setLogPath(std::string(argv[4]));
    }

    std::thread t1(&PeerListener::startListening, PeerListener());
    t1.detach();

    while (true)
    {
        std::cout << "Enter Command..." << std::endl;
        // cout << "in while in client main()" << endl;
        std::string command;
        std::getline(std::cin, command);
        // cout << "in while in client main() got command" << command <<endl;
        std::thread t1(&CommandHandler::handleCommand, CommandHandler(), command);
        t1.detach();
    }
}