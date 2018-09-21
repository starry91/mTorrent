
#include "utils.h"
#include <arpa/inet.h>
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