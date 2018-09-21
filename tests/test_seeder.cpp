
// Client side C/C++ program to demonstrate Socket programming
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

int main(int argc, char const *argv[])
{
    syslog(0,"--------------------------------------------------");
    struct sockaddr_in address;
    int sock = 0, valread;
    struct sockaddr_in serv_addr;
    char buffer[1024] = {0};
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("\n Socket creation error \n");
        return -1;
    }
    std::cout << "Hello1" << std::endl;
    memset(&serv_addr, '0', sizeof(serv_addr));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    // Convert IPv4 and IPv6 addresses from text to binary form
    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0)
    {
        printf("\nInvalid address/ Address not supported \n");
        return -1;
    }
    std::cout << "Hello2" << std::endl;
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        printf("\nConnection Failed \n");
        return -1;
    }
    std::cout << "Hello3" << std::endl;
    char key[1024] = "get_seeds";
    printf(" file len: %d", strlen(key));
    long key_count = htonl(strlen(key));
    printf(" file len: %d", key_count);
    send(sock, &key_count, sizeof(key_count), 0);
    send(sock, key, strlen(key), 0);
    std::cout << "Hello4" << std::endl;
    // char buff[1024] = "sample.txt";
    // long byte_count = htonl(strlen(buff));
    // printf(" file len: %d", byte_count);
    // send(sock, &byte_count, sizeof(byte_count), 0);
    // send(sock, buff, strlen(buff), 0);

    char hash[] = "35ds43342##433";
    long hash_count = htonl(strlen(hash));
    printf(" hash len: %d", strlen(hash));
    send(sock, &hash_count, sizeof(hash_count), 0);
    send(sock, hash, strlen(hash), 0);
    std::cout << "Hello5" << std::endl;

    // char ip[1024] = "191.144.34.23";
    // long ip_count = htonl(strlen(ip));
    // printf(" file len: %d", ip_count);
    // send(sock, &ip_count, sizeof(ip_count), 0);
    // send(sock, ip, strlen(ip), 0);

    // char port[] = "9897";
    // long port_count = htonl(strlen(port));
    // printf(" hash len: %d", port_count);
    // send(sock, &port_count, sizeof(port_count), 0);
    // send(sock, port, strlen(hash), 0);
    Client c(sock);
    std::string temp = c.extractPayload();
    std::cout << "temp: " << temp << std::endl;
    int size = stoi(temp);
    std::cout << "msg: " << size << std::endl;
    for (int i = 0; i < size; i++)
    {
        printf("ip: %s", c.extractPayload().c_str());
        printf("port: %s", c.extractPayload().c_str());
    }
    printf("%s\n", buffer);
    return 0;
}