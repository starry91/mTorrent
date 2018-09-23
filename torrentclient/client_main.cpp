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

    std::cout << "tracker1 ip: " << tracker1.getIp() << "tracker1 port: " << tracker1.getPort() << std::endl;
    std::cout << "tracker2 ip: " << tracker2.getIp() << "tracker2 port: " << tracker2.getPort() << std::endl;

    ClientDatabase::getInstance().sethost(host);
    ClientDatabase::getInstance().setTracker1(tracker1);
    ClientDatabase::getInstance().setTracker2(tracker2);

    if (argc == 5)
    {
        cout << "got log path" << endl;
        ClientDatabase::getInstance().setLogPath(std::string(argv[4]));
        LogHandler::getInstance().setLogPath(std::string(argv[4]));
    }

    std::thread t1(&PeerListener::startListening, PeerListener());
    t1.detach();

    while (true)
    {
        cout << "in while in client main()" << endl;
        std::string command;
        std::getline(std::cin, command);
        cout << "in while in client main() got command" << command <<endl;
        std::thread t1(&CommandHandler::handleCommand, CommandHandler(), command);
        t1.detach();
    }
}

// #include <unistd.h>
// #include <arpa/inet.h>
// #include "client.h"
// #define PORT 8080
// int main()
// {
//     syslog(0, "--------------------------------------------------");
//     struct sockaddr_in address;
//     int sock = 0, valread;
//     struct sockaddr_in serv_addr;
//     char buffer[1024] = {0};
//     if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
//     {
//         printf("\n Socket creation error \n");
//         return -1;
//     }
//     std::cout << "Hello1" << std::endl;
//     memset(&serv_addr, '0', sizeof(serv_addr));

//     serv_addr.sin_family = AF_INET;
//     serv_addr.sin_port = htons(PORT);

//     // Convert IPv4 and IPv6 addresses from text to binary form
//     if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0)
//     {
//         printf("\nInvalid address/ Address not supported \n");
//         return -1;
//     }
//     std::cout << "Hello2" << std::endl;
//     if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
//     {
//         printf("\nConnection Failed \n");
//         return -1;
//     }

//     Share msg1;
//     msg1.setFileName("foo.txt");
//     msg1.setHash("xyz");
//     msg1.setIp("192.168.21.22");
//     msg1.setPort("9090");

//     TrackerServiceServer t1(sock);
//     auto res1 = t1.shareFile(msg1);
//     cout << res1.getResponse() << endl;

//     AddSeeder msg2;
//     msg2.setHash("xyz");
//     msg2.setIp("192.168.21.22");
//     msg2.setPort("9090");

//     auto res2 = t1.addSeederRequest(msg2);
//     cout << res2.getResponse() << endl;

//     msg2.setHash("xyz222");
//     msg2.setIp("193.128.21.22");
//     msg2.setPort("9092");

//     res2 = t1.addSeederRequest(msg2);
//     cout << res2.getResponse() << endl;

//     RemoveSeeder msg3;
//     msg3.setHash("xyz");
//     msg3.setIp("192.168.21.22");
//     msg3.setPort("9090");

//     auto res3 = t1.removeSeederRequest(msg3);
//     cout << res3.getResponse() << endl;

//     SeederInfoRequest msg4;
//     msg4.setHash("xyzdsaaaaaaaaaadaswdasdasdsadsadasdasdsadasdasdasdsadsadasd");

//     SeederInfoResponse seeder_list = t1.getSeederInfo(msg4);
//     cout << seeder_list.getStatus() << endl;
//     cout << (seeder_list.getHash() == msg4.getHash()) << endl;

//     printf("%s\n", buffer);
//     return 0;
// }