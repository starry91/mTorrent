#include "client.h"
#include <sys/socket.h>
#include <unistd.h>

std::string Client::extractPayload()
{
    char size_buffer[1024];
    int valread = read(fd, size_buffer, 32);
    size_buffer[32] = 0;
    int32_t byte_count = atoi(size_buffer);
    char payload[1024];
    read(fd, payload, byte_count);
    payload[byte_count] = 0;
    return std::string(payload);
}

Client::Client(int fd) {
    this->fd = fd;
}