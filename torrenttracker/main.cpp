#include <iostream>
#include <vector>
#include <string.h>
#include "seeder.h"
#include <thread>
#include <sstream>
#include <syslog.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "trackerDatabase.h"
#include "rpcHandler.h"
#include "logHandler.h"

int main(int argc, char *argv[])
{
    syslog(0, "--------------------------------------------------");
    if (argc < 5)
    {
        LogHandler::getInstance().logError("Not enough arguments");
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
    LogHandler::getInstance().setLogPath(std::string(argv[4]));
    LogHandler::getInstance().logMsg("Client started");
    //throws logic error
    // Seeder tracker1(std::string(strtok(argv[1], ":")), std::string(strtok(NULL, ":")));
    // Seeder tracker2(std::string(strtok(argv[2], ":")), std::string(strtok(NULL, ":")));

    std::cout << "hello" << std::endl;
    std::cout << "ip: " << tracker1.getIp() << " port: " << tracker1.getPort() << std::endl;
    std::cout << "ip: " << tracker2.getIp() << " port: " << tracker2.getPort() << std::endl;

    TrackerDatabase::getInstance().setLogPath(std::string(argv[4]));
    TrackerDatabase::getInstance().setMainTracker(tracker1);
    TrackerDatabase::getInstance().setSecondaryTracker(tracker2);
    TrackerDatabase::getInstance().setSeederFilePath(std::string(argv[3]));
    TrackerDatabase::getInstance().readSeederfile();

    int server_fd, opt = 1;
    struct sockaddr_in server_addr, client_addr;
    socklen_t len;
    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,
                   &opt, sizeof(opt)))
    {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    memset(&server_addr, 0, sizeof(server_addr));
    memset(&client_addr, 0, sizeof(client_addr));

    // Filling server information
    server_addr.sin_family = AF_INET; // IPv4
    server_addr.sin_addr.s_addr = inet_addr(TrackerDatabase::getInstance().getMainTracker().getIp().c_str());
    server_addr.sin_port = htons(stoi(TrackerDatabase::getInstance().getMainTracker().getPort()));

    // Bind the socket with the server address
    if (bind(server_fd, (const struct sockaddr *)&server_addr,
             sizeof(server_addr)) < 0)
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    if (listen(server_fd, 10) < 0)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }
    while (true)
    {
        int cli_fd;
        if ((cli_fd = accept(server_fd, (struct sockaddr *)&client_addr,
                             (socklen_t *)&client_addr)) < 0)
        {
            perror("accept");
            exit(EXIT_FAILURE);
        }
        std::thread t1(&RpcHandler::handleRpc, RpcHandler(), cli_fd);
        t1.detach();
    }
}

// #include "message.h"

// int main2()
// {
//     //Share
//     std::cout << "Share---------------" << std::endl;
//     Share s1;
//     s1.setHash("xyz");
//     s1.setIp("a.b.c.d");
//     s1.setPort("1234");
//     auto b = s1.getBytes();
//     Share s2(b);
//     std::cout << (s1.getIp() == s2.getIp()) << std::endl;
//     std::cout << (s1.getPort() == s2.getPort()) << std::endl;
//     std::cout << s2.getType() << std::endl;

//     std::cout << "Add Seeder---------------" << std::endl;

//     AddSeeder as1;
//     as1.setHash("xyz");
//     as1.setIp("a.b.c.d");
//     as1.setPort("1234");
//     auto ab = as1.getBytes();
//     AddSeeder as2(ab);
//     std::cout << (as1.getIp() == as2.getIp()) << std::endl;
//     std::cout << (as1.getPort() == as2.getPort()) << std::endl;
//     std::cout << (as1.getHash() == as2.getHash()) << std::endl;
//     std::cout << as2.getType() << std::endl;

//     std::cout << "Remove Seeder---------------" << std::endl;

//     RemoveSeeder rs1;
//     rs1.setHash("xyz");
//     rs1.setIp("a.b.c.d");
//     rs1.setPort("1234");
//     auto rb = rs1.getBytes();
//     RemoveSeeder rs2(ab);
//     std::cout << (rs1.getIp() == rs2.getIp()) << std::endl;
//     std::cout << (rs1.getPort() == rs2.getPort()) << std::endl;
//     std::cout << (rs1.getHash() == rs2.getHash()) << std::endl;
//     std::cout << rs2.getType() << std::endl;

//     std::cout << "Seeder Info Request---------------" << std::endl;

//     SeederInfoRequest sir1;
//     sir1.setHash("xyz");
//     auto sirb = sir1.getBytes();
//     SeederInfoRequest sir2(sirb);
//     std::cout << (sir1.getHash() == sir2.getHash()) << std::endl;
//     std::cout << sir2.getType() << std::endl;

//     std::cout << "Seeder Info Response---------------" << std::endl;

//     SeederInfoResponse sires1;
//     sires1.setHash("xyz");
//     sires1.addSeeder(Seeder("223.232.11.22", "3333"));
//     auto siresb = sires1.getBytes();
//     SeederInfoResponse sires2(siresb);
//     std::cout << (sires1.getHash() == sires2.getHash()) << std::endl;
//     std::cout << (sires1.getSeeders()[0].getIp() == sires2.getSeeders()[0].getIp()) << std::endl;
//     std::cout << sires1.getSeeders()[0].getPort() << " : " << sires2.getSeeders()[0].getPort() << std::endl;
//     std::cout << sires2.getType() << std::endl;

//     std::cout << "Chunk Info Request---------------" << std::endl;

//     ChunkInfoRequest cr1;
//     cr1.setHash("xyz");
//     auto crb = cr1.getBytes();
//     ChunkInfoRequest cr2(crb);
//     std::cout << (cr1.getHash() == cr2.getHash()) << std::endl;
//     std::cout << cr2.getType() << std::endl;

//     std::cout << "ChunkInfo Response ---------------" << std::endl;

//     ChunkInfoResponse crr1;
//     crr1.setHash("xyz");
//     crr1.setChunkInfo("101001");
//     auto crrb = crr1.getBytes();
//     ChunkInfoResponse crr2(crrb);
//     std::cout << (crr1.getHash() == crr2.getHash()) << std::endl;
//     std::cout << (crr1.getChunkInfo() == crr2.getChunkInfo()) << std::endl;
//     std::cout << crr2.getType() << std::endl;

//     std::cout << "SendChunkRequest ---------------" << std::endl;

//     SendChunkRequest scr1;
//     scr1.setHash("xyz");
//     scr1.setChunkId("1");
//     auto scrb = scr1.getBytes();
//     SendChunkRequest scr2(scrb);
//     std::cout << (scr1.getHash() == scr2.getHash()) << std::endl;
//     std::cout << (scr1.getChunkId() == scr2.getChunkId()) << std::endl;
//     std::cout << scr2.getType() << std::endl;

//     std::cout << "SendChunkResponse ---------------" << std::endl;

//     SendChunkResponse scres1;
//     scres1.setHash("xyz");
//     scres1.setChunkIndex("1");
//     auto scresb = scres1.getBytes();
//     SendChunkResponse scres2(scresb);
//     std::cout << (scres1.getHash() == scres2.getHash()) << std::endl;
//     std::cout << (scres1.getChunkIndex() == scres2.getChunkIndex()) << std::endl;
//     std::cout << scres2.getType() << std::endl;

//     std::cout << "Response ---------------" << std::endl;

//     Response res1;
//     res1.setResponse("SUCCESS");
//     auto resb = res1.getBytes();
//     Response res2(resb);
//     std::cout << (res1.getResponse() == res2.getResponse()) << std::endl;
//     std::cout << res2.getType() << std::endl;
// }