#include "tracker.h"
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

void Tracker::startListening(std::string ip, int port)
{
    int sockfd, server_fd, opt = 1;
    struct sockaddr_in server_addr, client_addr;
    socklen_t len;
    // Creating socket file descriptor
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
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
    server_addr.sin_addr.s_addr = inet_addr(ip.c_str());
    server_addr.sin_port = htons(port);

    // Bind the socket with the server address
    if (bind(sockfd, (const struct sockaddr *)&server_addr,
             sizeof(server_addr)) < 0)
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    if (listen(server_fd, 3) < 0)
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
        std::thread t1(serviceRequest, cli_fd);
    }
};

void Tracker::serviceRequest(int client_fd)
{
    Client client(client_fd);
    if (client.extractPayload() == "add_file")
    {
        add_file(client_fd);
    }
    else if (client.extractPayload() == "add_seeder")
    {
        add_seeder(client_fd);
    }
}

void Tracker::add_file(int fd)
{
    Client client(fd);
    std::string file_name = client.extractPayload();
    std::string hash = client.extractPayload();
    std::string ip = client.extractPayload();
    std::string port = client.extractPayload();
    auto file_attr = std::make_shared<FileAttr>(hash, Seeder(ip, port));
    if (this->files.find(file_name) != this->files.end())
    {
        //error
    }
    else
    {
        this->files[file_name] = file_attr;
    }
};

void Tracker::add_seeder(int fd)
{
    Client client(fd);
    std::string file_name = client.extractPayload();
    std::string ip = client.extractPayload();
    std::string port = client.extractPayload();
    auto seeder = std::make_shared<Seeder>(Seeder(ip, port));
    auto file = this->files[file_name];
    file->addSeeder(seeder);
}

void Tracker::remove_seeder(int fd)
{
    Client client(fd);
    std::string file_name = client.extractPayload();
    std::string ip = client.extractPayload();
    std::string port = client.extractPayload();
    auto seeder = std::make_shared<Seeder>(Seeder(ip, port));
    this->files[file_name]->removeSeeder(seeder);
    if(this->files[file_name]->getSeederCount() == 0) {
        this->files.erase(file_name);
    }
}

void add_file(std::string, std::shared_ptr<FileAttr>);
void add_seeder(seeder_Sptr);
void remove_seeder(seeder_Sptr);
void remove_file(std::string);
