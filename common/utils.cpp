
#include "utils.h"
#include <arpa/inet.h>
#include <unistd.h>
#include <istream>
#include <cstring>
#include <fstream>
#include <limits>
#include <math.h>
#include "openssl/sha.h"
#include <syslog.h>
#include "mtorrent.h"
#include <syslog.h>
#include "clientDatabase.h"

uint32_t nvtouint32(const std::vector<char> &arr)
{
    uint32_t hsize;
    int i = 0;
    for (; i < sizeof(hsize); i++)
    {
        // std::cout << "char: " << (int)arr[i] << std::endl;
        ((char *)(&hsize))[i] = arr[i];
    }
    hsize = ntohl(hsize);
    return hsize;
}

std::vector<char> uint32tonv(uint32_t val)
{
    std::vector<char> buf;
    val = htonl(val);

    for (int i = 0; i < sizeof(val); i++)
    {
        // std::cout << "charbyte: " << (int)((char *)(&val))[i] << std::endl;
        buf.push_back(((char *)(&val))[i]);
    }
    return buf;
}

std::vector<char> readBytes(int n, int sock_fd)
{
    const uint32_t BUFSIZE = 1024 * 1024;
    char buf[BUFSIZE];
    int count = 0;
    std::vector<char> ebuf;
    while (count < n)
    {
        int temp_count = read(sock_fd, buf, BUFSIZE > (n - count) ? n - count : BUFSIZE);
        for (int i = 0; i < temp_count; i++)
        {
            ebuf.push_back(buf[i]);
        }
        count += temp_count;
    }
    return ebuf;
}

std::vector<std::string> extractArgs(std::string command)
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

//generating mtorrent file
void createMTorrent(mTorrent_Sptr torr)
{
    std::ofstream myfile(torr->getfileName().c_str());
    if (myfile.is_open())
    {
        syslog(0, "Writing file to: [%s]", torr->getPath().c_str());
        myfile << torr->getHash() << "\n";
        myfile << torr->getPath() << "\n";
        myfile << torr->getFileSize() << "\n";
        myfile << ClientDatabase::getInstance().getTracker1().getIp() << "\n";
        myfile << ClientDatabase::getInstance().getTracker1().getPort() << "\n";
        myfile << ClientDatabase::getInstance().getTracker2().getIp() << "\n";
        myfile << ClientDatabase::getInstance().getTracker2().getPort() << "\n";
    }
    myfile.close();
}

//getting the hash of the file
std::string getFileHash(std::string file_name)
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

long fileSize(std::string path)
{
    std::ifstream file;
    file.open(path, std::ios::in | std::ios::binary);
    file.ignore(std::numeric_limits<std::streamsize>::max());
    std::streamsize file_size = file.gcount();
    file.close();
    return file_size;
}

int createTCPClient(Seeder client)
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
    serv_addr.sin_port = htons(std::stoi(client.getPort()));

    // Convert IPv4 and IPv6 addresses from text to binary form
    if (inet_pton(AF_INET, client.getIp().c_str(), &serv_addr.sin_addr) <= 0)
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
    return sock;
}