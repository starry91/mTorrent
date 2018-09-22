
#include "utils.h"
#include <arpa/inet.h>
#include <unistd.h>
#include <istream>

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
  const uint32_t BUFSIZE = 1024*1024;
  char buf[BUFSIZE];
  int count = 0;
  std::vector<char> ebuf;
  while (count < n)
  {
    int temp_count = read(sock_fd, buf, BUFSIZE > (n - count) ? n - count : BUFSIZE);
    for(int i=0;i<temp_count;i++) {
      ebuf.push_back(buf[i]);
    }
    count += temp_count;
  }
  return ebuf;
}