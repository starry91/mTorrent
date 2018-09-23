//Name: Praveen Balireddy
//Roll: 2018201052

#ifndef TRACKERSERVICESERVER_H
#define TRACKERSERVICESERVER_H

#include <string>
#include <iostream>
#include "message.h"
#include "seeder.h"
#include <vector>

class TrackerServiceServer
{
  int tracker_fd;

public:
  TrackerServiceServer();
  Response shareFile(Share msg);
  Response addSeederRequest(AddSeeder msg);
  Response removeSeederRequest(RemoveSeeder msg);
  SeederInfoResponse getSeederInfo(SeederInfoRequest msg);
  void connectToTracker();
};

#endif