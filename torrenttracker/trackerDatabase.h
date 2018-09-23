//Name: Praveen Balireddy
//Roll: 2018201052

#ifndef TRACKERDATABASE_H
#define TRACKERDATABASE_H

#include <string>
#include <iostream>
#include <map>
#include "fileAttr.h"
#include "seeder.h"
#include <mutex>
#include <condition_variable>

class TrackerDatabase
{
private:
  std::mutex seeder_mtx;             // mutex for critical section
  Seeder tracker1;
  Seeder tracker2;
  std::string log_path;
  std::string seeder_file_path;
  std::map<std::string, std::shared_ptr<FileAttr>> files;
  TrackerDatabase();

public:
  static TrackerDatabase &getInstance();
  void setMainTracker(Seeder tracker);
  void setSecondaryTracker(Seeder tracker);
  void setLogPath(std::string path);
  void setSeederFilePath(std::string path);
  void addFileEntry(std::shared_ptr<FileAttr> file);
  void addSeeder(std::string hash, seeder_Sptr seeder);
  void remove_seeder(std::string hash, seeder_Sptr seeder);
  std::vector<seeder_Sptr> getSeederList(std::string hash);
  Seeder getMainTracker();
  bool exists(std::string hash);
  void readSeederfile();
};

#endif