#include "clientHandler.h"
#include <string.h>
#include <thread>
#include <fstream>
#include <limits>
#include <math.h>
#include "openssl/sha.h"
#include <syslog.h>
#define CHUNK_SIZE 524288

ClientHandler::ClientHandler(seeder_Sptr host, seeder_Sptr tracker1, seeder_Sptr tracker2)
{
    this->host = host;
    this->tracker1 = tracker1;
    this->tracker2 = tracker2;
}

std::vector<std::string> ClientHandler::extractArgs(std::string command)
{
    //char str[] = "- This, a sample string.";
    char *pch;
    std::vector<std::string> args;
    pch = strtok((char *)command.c_str(), " ");
    //args.push_back(std::string(pch));
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
        std::cout << this->getFileHash(args[1]) << std::endl;

        // this->createMTorrent
        //     std::thread t1(&ClientHandler::shareFile, this, args[1]);
    }
}

void ClientHandler::createMTorrent(std::string local_file, std::string torrent_name)
{
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
