
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
    cout << "Called readBytes fd: " << sock_fd << ", n: " << n << endl;
    while (count < n)
    {
        cout << "before read: count: " << count << ", n: "<< n << endl;
        int temp_count = read(sock_fd, buf, BUFSIZE > (n - count) ? n - count : BUFSIZE);
        cout << "after read: tmp_count: " << temp_count << endl;
        if(temp_count == 0) {
            throw ErrorMsg("connection closed");
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
    cout << "createTCPClient() Connected to Tracker with fd: " << sock << endl;
    return sock;
}