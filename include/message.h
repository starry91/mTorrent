//Name: Praveen Balireddy
//Roll: 2018201052

#ifndef MESSAGE_H
#define MESSAGE_H

#include <string>
#include <iostream>
#include <memory>
#include <vector>
#include "utils.h"
#include "seeder.h"

//------------------------------------------------------------Base Class: Message----------------------------------------------------------------------------
class Message
{
public:
  virtual std::string getType() = 0;
  virtual std::vector<char> getBytes() = 0;
};

//------------------------------------------------------------Message: Share----------------------------------------------------------------------------
class Share : public Message
{
  std::string file_name;
  std::string hash;
  std::string ip;
  std::string port;

public:
  std::string getHash();
  std::string getIp();
  std::string getPort();
  std::string getFileName();
  Share(std::vector<char>);
  Share();
  void setHash(std::string);
  void setIp(std::string);
  void setPort(std::string);
  void setFileName(std::string);
  virtual std::string getType();
  virtual std::vector<char> getBytes();
};

//------------------------------------------------------------Message: AddSeeder----------------------------------------------------------------------------
class AddSeeder : public Message
{
  std::string hash;
  std::string ip;
  std::string port;

public:
  std::string getHash();
  std::string getIp();
  std::string getPort();
  AddSeeder(std::vector<char>);
  AddSeeder();
  void setHash(std::string);
  void setIp(std::string);
  void setPort(std::string);
  virtual std::string getType();
  virtual std::vector<char> getBytes();
};

//------------------------------------------------------------Message: RemoveSeeder----------------------------------------------------------------------------
class RemoveSeeder : public Message
{
  std::string hash;
  std::string ip;
  std::string port;

public:
  std::string getHash();
  std::string getIp();
  std::string getPort();
  RemoveSeeder(std::vector<char>);
  RemoveSeeder();
  void setHash(std::string);
  void setIp(std::string);
  void setPort(std::string);
  virtual std::string getType();
  virtual std::vector<char> getBytes();
};

//------------------------------------------------------------Message: SeederInfoRequest----------------------------------------------------------------------------
class SeederInfoRequest : public Message
{
  std::string hash;

public:
  std::string getHash();
  SeederInfoRequest(std::vector<char>);
  SeederInfoRequest();
  void setHash(std::string);
  virtual std::string getType();
  virtual std::vector<char> getBytes();
};

//------------------------------------------------------------Message: SeederInfoResponse----------------------------------------------------------------------------
class SeederInfoResponse : public Message
{
  std::string hash;
  std::vector<Seeder> seeder_list;
  std::string status;

public:
  SeederInfoResponse(std::vector<char> b);
  SeederInfoResponse();
  void addSeeder(Seeder seeder);
  void setHash(std::string hash);
  void setStatus(std::string status);
  std::vector<Seeder> getSeeders();
  std::string getHash();
  std::string getStatus();
  virtual std::string getType();
  virtual std::vector<char> getBytes();
};

//------------------------------------------------------------Message: ChunkInfoRequest----------------------------------------------------------------------------
class ChunkInfoRequest : public Message
{
  std::string hash;

public:
  std::string getHash();
  ChunkInfoRequest(std::vector<char>);
  ChunkInfoRequest();
  void setHash(std::string);
  virtual std::string getType();
  virtual std::vector<char> getBytes();
};

//------------------------------------------------------------Message: ChunkInfoResponse----------------------------------------------------------------------------
class ChunkInfoResponse : public Message
{
  std::string hash;
  std::string chunk_map;

public:
  std::string getChunkInfo();
  std::string getHash();
  ChunkInfoResponse(std::vector<char> chunks);
  ChunkInfoResponse();
  void setChunkInfo(std::string chunk_map);
  void setHash(std::string hash);
  virtual std::string getType();
  virtual std::vector<char> getBytes();
};

//------------------------------------------------------------Message: SendChunkRequest----------------------------------------------------------------------------
class SendChunkRequest : public Message
{
  std::string hash;
  std::string chunk_index;

public:
  std::string getChunkId();
  std::string getHash();
  SendChunkRequest(std::vector<char>);
  SendChunkRequest();
  void setChunkId(std::string chunk_index);
  void setHash(std::string hash);
  virtual std::string getType();
  virtual std::vector<char> getBytes();
};

//------------------------------------------------------------Message: SendChunkResponse----------------------------------------------------------------------------
class SendChunkResponse : public Message
{
  std::string hash;
  std::string chunk_index;
  std::vector<char> chunk_data;

public:
  std::vector<char> getChunkdata();
  std::string getHash();
  std::string getChunkIndex();
  SendChunkResponse(std::vector<char> &b);
  SendChunkResponse();
  void setChunkData(std::vector<char> &data);
  void setHash(std::string hash);
  void setChunkIndex(std::string index);
  virtual std::string getType();
  virtual std::vector<char> getBytes();
};

//------------------------------------------------------------Message: Response(SUCCESS/FAIL)----------------------------------------------------------------------------
class Response : public Message
{
  std::string response;

public:
  std::string getResponse();
  Response(std::vector<char> &b);
  Response();
  void setResponse(std::string response);
  virtual std::string getType();
  virtual std::vector<char> getBytes();
};

#endif