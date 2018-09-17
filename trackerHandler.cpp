#include "trackerHandler.h"
#include <json/json.h>
#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <thread>
#include <unistd.h>
#include "client.h"
#include "seeder.h"
#include "fileAttr.h"
#include <fstream>
#include <mutex>
#include <condition_variable>
#include <syslog.h>

std::mutex seeder_mtx;             // mutex for critical section
std::condition_variable seeder_cv; // condition variable for critical section

TrackerHandler::TrackerHandler(Seeder tracker1, Seeder tracker2)
{
    this->tracker1 = tracker1;
    this->tracker2 = tracker2;
}

void TrackerHandler::startListening()
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
    server_addr.sin_addr.s_addr = inet_addr(this->tracker1.getIp().c_str());
    server_addr.sin_port = htons(stoi(this->tracker1.getPort()));

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
        std::thread t1(&TrackerHandler::serviceRequest, this, cli_fd);
        t1.detach();
    }
};

void TrackerHandler::serviceRequest(int client_fd)
{
    Client client(client_fd);
    if (client.extractPayload() == "add_file")
    {
        std::cout << "hello1" << std::endl;
        add_file(client_fd);
    }
    else if (client.extractPayload() == "add_seeder")
    {
        std::cout << "hello2" << std::endl;
        add_seeder(client_fd);
    }
    else if (client.extractPayload() == "remove_seeder")
    {
        std::cout << "hello3" << std::endl;
        remove_seeder(client_fd);
    }
    else if (client.extractPayload() == "get_seeds")
    {
        std::cout << "hello4" << std::endl;
        add_seeder(client_fd);
    }
}

void TrackerHandler::add_file(int fd)
{
    Client client(fd);
    std::string file_name = client.extractPayload();
    std::string hash = client.extractPayload();
    std::string ip = client.extractPayload();
    std::string port = client.extractPayload();
    auto file_attr = std::make_shared<FileAttr>(FileAttr(file_name, hash, std::make_shared<Seeder>(Seeder(ip, port))));
    if (this->files.find(hash) != this->files.end())
    {
        //error
    }
    else
    {
        this->files[hash] = file_attr;
    }
};

void TrackerHandler::add_seeder(int fd)
{
    Client client(fd);
    std::string hash = client.extractPayload();
    std::string ip = client.extractPayload();
    std::string port = client.extractPayload();
    auto seeder = std::make_shared<Seeder>(Seeder(ip, port));
    {
        std::lock_guard<std::mutex> lock(seeder_mtx);
        auto file = this->files[hash];
        file->addSeeder(seeder);
    }
}

void TrackerHandler::remove_seeder(int fd)
{
    Client client(fd);
    std::string hash = client.extractPayload();
    std::string ip = client.extractPayload();
    std::string port = client.extractPayload();
    auto seeder = std::make_shared<Seeder>(Seeder(ip, port));
    {
        std::lock_guard<std::mutex> lock(seeder_mtx);
        this->files[hash]->removeSeeder(seeder);
        if (this->files[hash]->getSeederCount() == 0)
        {
            this->files.erase(hash);
        }
    }
}

void TrackerHandler::sendSeedList(int fd)
{

    Client client(fd);
    std::string hash = client.extractPayload();
    {
        std::lock_guard<std::mutex> lock(seeder_mtx);
        client.sendSeederData(this->files[hash]->getSeeds());
    }
}

void TrackerHandler::readSeederfile(std::string file)
{
    std::string line;
    std::ifstream input(file);
    if (input.is_open())
    {
        bool eof = false;
        while (!eof)
        {
            std::string file_name, hash;
            std::vector<seeder_Sptr> seeds;
            if (std::getline(input, line))
            {
                file_name = std::string(line);
            }
            if (std::getline(input, line))
            {
                hash = std::string(line);
            }
            while (std::getline(input, line))
            {
                //syslog(0, "seeder: [%s]", line.c_str());
                if (std::string(line) != "")
                    seeds.push_back(std::make_shared<Seeder>(Seeder(std::string(line))));
                else
                    break;
            }
            if (seeds.size() != 0)
                this->files[hash] = std::make_shared<FileAttr>(FileAttr(file_name, hash, seeds));
            if (input.eof())
                eof = true;
            //syslog(0, "eof: %d", eof);
            //syslog(0, "Filename: %s Hash: %s, seeds: %d", file_name.c_str(), hash.c_str(), seeds.size());
            //syslog(0, "Seeder: %s", seeds[6]);
        }
        input.close();
    }
}

void TrackerHandler::setLogPath(std::string path)
{
    this->log_path = path;
}
