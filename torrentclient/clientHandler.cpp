#include "clientHandler.h"
#include <string.h>
#include <thread>
#include <fstream>
#include <limits>
#include <math.h>
#include "openssl/sha.h"
#include <syslog.h>
#include "mtorrent.h"
#include <memory>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include "client.h"
#define PORT 8080
#include <syslog.h>
#include <sys/socket.h>
#include "message.h"
#define CHUNK_SIZE 524288

ClientHandler::ClientHandler(seeder_Sptr host, seeder_Sptr tracker1, seeder_Sptr tracker2)
{
    this->host = host;
    this->tracker1 = tracker1;
    this->tracker2 = tracker2;
    this->connectToTracker();
}

std::vector<std::string> ClientHandler::extractArgs(std::string command)
{
    char *pch;
    std::vector<std::string> args;
    pch = strtok((char *)command.c_str(), " ");
    while (pch != NULL)
    {
        args.push_back(std::string(pch));
        pch = strtok(NULL, " ");
    }
    return args;
}

void ClientHandler::handleCommand(std::string command)
{
    std::vector<std::string> args = this->extractArgs(command);
    for (int i = 0; i < args.size(); i++)
    {
        syslog(0, "arg: [%s]", args[i].c_str());
    }
    if (args[0] == "share")
    {
        this->shareFile(args);
    }
    else if (args[0] == "get")
    {
        auto seeders = this->getSeeders(args[1]);
    }
}

void ClientHandler::shareFile(std::vector<std::string> args)
{
    std::ifstream file;
    file.open(args[1], std::ios::in | std::ios::binary);
    file.ignore(std::numeric_limits<std::streamsize>::max());
    std::streamsize file_size = file.gcount();
    file.close();

    std::string file_hash = this->getFileHash(args[1]);
    std::cout << file_hash << std::endl;
    std::string bit_chunks = "";
    for (int i = 0; i < file_hash.length() / SHA_DIGEST_LENGTH; i++)
    {
        bit_chunks += "1";
    }
    syslog(0, "Bit Chunks: [%s]", bit_chunks.c_str());
    auto mtorr = std::make_shared<mTorrent>(mTorrent(args[1], file_hash, args[2], file_size, bit_chunks));
    this->createMTorrent(mtorr);
    this->addMTorrent(mtorr);
    this->shareToTracker(file_hash);
    // this->createMTorrent
    //     std::thread t1(&ClientHandler::shareFile, this, args[1]);
}

std::vector<seeder_Sptr> ClientHandler::getSeeders(std::string hash)
{
    auto torr = this->files[hash];

    //sending request type
    char key[1024] = "get_seeds";
    long key_count = htonl(strlen(key));
    send(this->tracker_fd, &key_count, sizeof(key_count), 0);
    send(this->tracker_fd, key, strlen(key), 0);

    //sending hash
    long hash_count = htonl(hash.length());
    send(this->tracker_fd, &hash_count, sizeof(hash_count), 0);
    send(this->tracker_fd, hash.c_str(), hash.length(), 0);

    Client c(this->tracker_fd);
    std::string temp = c.extractPayload();
    std::cout << "temp: " << temp << std::endl;
    int size = stoi(temp);
    std::cout << "msg: " << size << std::endl;
    for (int i = 0; i < size; i++)
    {
        printf("ip: %s", c.extractPayload().c_str());
        printf("port: %s", c.extractPayload().c_str());
    }
}

void ClientHandler::createMTorrent(mTorrent_Sptr torr)
{
    std::ofstream myfile(torr->getfileName().c_str());
    if (myfile.is_open())
    {
        syslog(0, "Writing file to: [%s]", torr->getPath().c_str());
        myfile << torr->getHash() << "\n";
        myfile << torr->getPath() << "\n";
        myfile << this->host->getIp() << "\n";
        myfile << this->host->getPort();
    }
    myfile.close();
}

std::string ClientHandler::getFileHash(std::string file_name)
{
    std::ifstream file;
    file.open(file_name, std::ios::in | std::ios::binary);
    file.ignore(std::numeric_limits<std::streamsize>::max());
    std::streamsize file_size = file.gcount();
    std::string hash = "";
    int chunk_count = ceil((file_size * 1.00) / CHUNK_SIZE);
    for (int i = 0; i < chunk_count; i++)
    {
        char data[CHUNK_SIZE + 1];
        file.seekg(i * CHUNK_SIZE);
        size_t length;
        if (i == chunk_count - 1)
        {
            file.read(data, file_size % CHUNK_SIZE);
            data[file_size % CHUNK_SIZE] = '\0';
            length = file_size % CHUNK_SIZE;
        }
        else
        {
            file.read(data, CHUNK_SIZE);
            data[CHUNK_SIZE] = '\0';
            length = CHUNK_SIZE;
        }

        unsigned char hash_buff[SHA_DIGEST_LENGTH];
        SHA1(reinterpret_cast<const unsigned char *>(data), length, hash_buff);

        char mdString[SHA_DIGEST_LENGTH * 2];

        for (int i = 0; i < SHA_DIGEST_LENGTH; i++)
        {
            sprintf(&mdString[i * 2], "%02x", (unsigned int)hash_buff[i]);
        }
        std::string chunk_hash = std::string((char *)mdString);
        hash += chunk_hash.substr(0, 20);
        syslog(0, "Chunk Hash: [%s]", chunk_hash.c_str());
    }
    return hash;
}

void ClientHandler::addMTorrent(mTorrent_Sptr torr)
{
    if (this->files.find(torr->getHash()) == this->files.end())
    {
        this->files[torr->getHash()] = torr;
    }
}

void ClientHandler::shareToTracker(std::string hash)
{
    auto torr = this->files[hash];

    //Share msg
    Share share;
    share.setFileName(torr->getfileName());
    share.setHash(torr->getfileName());
    share.setIp(this->host->getIp());
    share.setIp(this->host->getPort());

    //sending request type
    char key[1024] = "add_file";
    long key_count = htonl(strlen(key));
    send(this->tracker_fd, &key_count, sizeof(key_count), 0);
    send(this->tracker_fd, key, strlen(key), 0);

    //sending file name
    long name_count = htonl(torr->getfileName().length());
    send(this->tracker_fd, &name_count, sizeof(name_count), 0);
    send(this->tracker_fd, torr->getfileName().c_str(), torr->getfileName().length(), 0);

    //sending hash
    long hash_count = htonl(hash.length());
    send(this->tracker_fd, &hash_count, sizeof(hash_count), 0);
    send(this->tracker_fd, hash.c_str(), hash.length(), 0);

    //sending ip
    long ip_count = htonl(this->host->getIp().length());
    send(this->tracker_fd, &ip_count, sizeof(ip_count), 0);
    send(this->tracker_fd, this->host->getIp().c_str(), this->host->getIp().length(), 0);

    //sending port
    long port_count = htonl(this->host->getPort().length());
    send(this->tracker_fd, &port_count, sizeof(port_count), 0);
    send(this->tracker_fd, this->host->getPort().c_str(), this->host->getPort().length(), 0);
}

void ClientHandler::connectToTracker()
{
    syslog(0, "Connecting to Tracker....");
    struct sockaddr_in address;
    int sock = 0, valread;
    struct sockaddr_in serv_addr;
    char buffer[1024] = {0};
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("\n Socket creation error \n");
        exit(2);
        //return -1;
    }
    memset(&serv_addr, '0', sizeof(serv_addr));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(std::stoi(this->tracker1->getPort()));

    // Convert IPv4 and IPv6 addresses from text to binary form
    if (inet_pton(AF_INET, this->tracker1->getIp().c_str(), &serv_addr.sin_addr) <= 0)
    {
        printf("\nInvalid address/ Address not supported \n");
        exit(2);
        //return -1;
    }
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        printf("\nConnection Failed \n");
        exit(2);
        //return -1;
    }
    syslog(0, "Connected to Tracker....");
    this->tracker_fd = sock;
}

void ClientHandler::startListeningToPeers()
{
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
    server_addr.sin_addr.s_addr = inet_addr(this->host->getIp().c_str());
    server_addr.sin_port = htons(stoi(this->host->getPort()));

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
        std::thread t1(&ClientHandler::serviceRequest, this, cli_fd);
        t1.detach();
    }
}

void ClientHandler::serviceRequest(int client_fd)
{
}