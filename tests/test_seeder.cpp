
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
#include "message.h"
#include "networkInterfacer.h"
#include "TrackerServiceServer.h"

int main(int argc, char const *argv[])
{
    syslog(0, "--------------------------------------------------");
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

    Share msg1;
    msg1.setFileName("foo.txt");
    msg1.setHash("xyz");
    msg1.setIp("192.168.21.22");
    msg1.setPort("9090");

    TrackerServiceServer t1(sock);
    t1.shareFile(msg1);

    AddSeeder msg2;
    msg2.setHash("xyz");
    msg2.setIp("192.168.21.22");
    msg2.setPort("9090");

    t1.addSeederRequest(msg2);

    RemoveSeeder msg3;
    msg3.setHash("xyz");
    msg3.setIp("192.168.21.22");
    msg3.setPort("9090");

    t1.removeSeederRequest(msg3);

    SeederInfoRequest msg4;
    msg4.setHash("xyz");

    std::vector<Seeder> seeder_list = t1.getSeederInfo(msg4);

    printf("%s\n", buffer);
    return 0;
}
