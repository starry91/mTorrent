#ifndef DOWNLOAD_H
#define DOWNLOAD_H

#include <iostream>
#include <string>
#include "mtorrent.h"
#include <memory>

class Download
{
  std::string hash;
  std::string file_name;
  int total_chunks;
  bool downloadStatus;
  int downloaded_chunks;
  std::vector<int> chunk_map;
  std::string download_path;

public:
  Download();
  Download(std::string hash, std::string file_name, std::string path, int tot_chunks);
  //setters
  void setHash(std::string);
  void setFileName(std::string);
  void setTotalChunks(int);
  std::string getHash();
  std::string getFileName();
  int getTotalChunks();
  void updateChunkStatus(int index, int val);
  void incrementDownloadedChunks();
};

typedef std::shared_ptr<Download> down_Sptr;

#endif