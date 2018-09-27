
#include "utils.h"
#include <arpa/inet.h>
#include <unistd.h>
#include <istream>
#include <cstring>
#include <fstream>
#include <limits>
#include <math.h>
#include <syslog.h>
#include "seeder.h"
#include "errorMsg.h"
#include "openssl/sha.h"

using std::cout;
using std::endl;

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
    //cout << "Called readBytes fd: " << sock_fd << ", n: " << n << endl;
    while (count < n)
    {
        // cout << "before read: count: " << count << ", n: " << n << endl;
        int temp_count = read(sock_fd, buf, BUFSIZE > (n - count) ? n - count : BUFSIZE);
        //cout << "after read: tmp_count: " << temp_count << endl;
        if (temp_count <= 0)
        {
            //cout << "readBytes() connection closed, fd: " + std::to_string(sock_fd) << endl;
            throw ErrorMsg("connection  , fd: " + std::to_string(sock_fd));
        }
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

int createTCPClient(Seeder client)
{
    cout << "In create TCP connection, Connecting to Tracker...." << endl;
    struct sockaddr_in address;
    int sock = 0, valread;
    struct sockaddr_in serv_addr;
    char buffer[1024] = {0};
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        throw ErrorMsg("Socket creation error");
    }
    cout << "In create TCP connection, socket created...." << endl;
    memset(&serv_addr, '0', sizeof(serv_addr));

    serv_addr.sin_family = AF_INET;
    cout << "In create TCP connection, port: " << client.getPort() << endl;
    serv_addr.sin_port = htons(std::stoi(client.getPort()));

    // Convert IPv4 and IPv6 addresses from text to binary form
    cout << "In create TCP connection, before itnet_pton...." << endl;
    if (inet_pton(AF_INET, client.getIp().c_str(), &serv_addr.sin_addr) <= 0)
    {
        throw ErrorMsg("Invalid address/ Address not supported");
    }
    cout << "In create TCP connection, set ip...." << endl;
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        throw ErrorMsg("Connection to " + client.getIp() + ":" + client.getPort() + " failed");
    }
    cout << "createTCPClient() Connected to Tracker with fd: " << sock << endl;
    return sock;
}

std::string getChunkHash(std::vector<char> bytes)
{
    std::string hash = "";
    unsigned char hash_buff[SHA_DIGEST_LENGTH];
    SHA1(reinterpret_cast<const unsigned char *>(bytes.data()), bytes.size(), hash_buff);

    char mdString[SHA_DIGEST_LENGTH * 2];

    for (int i = 0; i < SHA_DIGEST_LENGTH; i++)
    {
        sprintf(&mdString[i * 2], "%02x", (unsigned int)hash_buff[i]);
    }
    std::string chunk_hash = std::string((char *)mdString);
    hash += chunk_hash.substr(0, 20);
    //syslog(0, "In Get Chunk Hash: Chunk Hash: [%s]", chunk_hash.c_str());
    return hash;
}