//Name: Praveen Balireddy
//Roll: 2018201052

#ifndef MESSAGE_H
#define MESSAGE_H

#include <string>
#include <iostream>
#include <memory>
#include <vector>
#include "utils.h"
class Message
{
public:
  virtual std::string getType() = 0;
  virtual std::vector<char> getBytes() = 0;
};

class Share : public Message
{
  std::string hash;
  std::string ip;
  std::string port;

public:
  std::string getHash();
  std::string getIp();
  std::string getPort();
  Share(std::vector<char>);
  Share();
  void setHash(std::string);
  void setIp(std::string);
  void setPort(std::string);
  virtual std::string getType();
  virtual std::vector<char> getBytes();
};


// typedef std::shared_ptr<Message> message_Sptr;

#endif