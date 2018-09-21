#include "message.h"
#include <unistd.h>
#include <arpa/inet.h>
#include <syslog.h>
#include <string.h>
#include "utils.h"

Share::Share(std::vector<char> b)
{
  {
    uint32_t hash_size = nvtouint32(std::vector<char>(&b[0], &b[4]));
    this->hash = std::string(&b[4], &b[4 + hash_size]);
    b.erase(b.begin(), b.begin() + 4 + hash_size);
  }
  {
    uint32_t hash_size = nvtouint32(std::vector<char>(&b[0], &b[4]));
    // std::cout << "hsize: " << hash_size << std::endl;
    this->ip = std::string(&b[4], &b[4 + hash_size]);
    b.erase(b.begin(), b.begin() + 4 + hash_size);
  }

  {
    uint32_t hash_size = nvtouint32(std::vector<char>(&b[0], &b[4]));
    // std::cout << "hsize: " << hash_size << std::endl;
    this->port = std::string(&b[4], &b[4 + hash_size]);
    b.erase(b.begin(), b.begin() + 4 + hash_size);
  }
}

Share::Share()
{
}

std::string Share::getHash()
{
  return this->hash;
}
std::string Share::getIp()
{
  return this->ip;
}
std::string Share::getPort()
{
  return this->port;
}

std::string Share::getType()
{
  return std::string("share");
}

std::vector<char> Share::getBytes()
{
  std::vector<char> buf;
  uint32_t hsize = this->hash.size();
  buf = uint32tonv(hsize);
  for (auto i : this->hash)
  {
    buf.push_back(i);
  }
  {
    auto tmp = uint32tonv(this->ip.size());
    buf.insert(buf.end(), tmp.begin(), tmp.end());
    for (auto i : this->ip)
    {
      buf.push_back(i);
    }
  }

  auto tmp = uint32tonv(this->port.size());
  buf.insert(buf.end(), tmp.begin(), tmp.end());
  for (auto i : this->port)
  {
    buf.push_back(i);
  }

  return buf;
}

void Share::setHash(std::string hash)
{
  this->hash = hash;
}
void Share::setIp(std::string ip)
{
  this->ip = ip;
}
void Share::setPort(std::string port)
{
  this->port = port;
}
