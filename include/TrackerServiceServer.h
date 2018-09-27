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
  TrackerServiceServer(Seeder tracker1, Seeder tracker2);
  TrackerServiceServer(Seeder tracker1);
  Response shareFile(Share msg);
  Response addSeederRequest(AddSeeder msg);
  Response removeSeederRequest(RemoveSeeder msg);
  Response syncshareFile(SyncShare msg);
  Response syncaddSeederRequest(SyncAddSeeder msg);
  Response syncremoveSeederRequest(SyncRemoveSeeder msg);
  ChunkInfoResponse getChunkInfo(ChunkInfoRequest req);
  SendChunkResponse getChunk(SendChunkRequest req);
  SeederInfoResponse getSeederInfo(SeederInfoRequest msg);
  SyncSeederListResponse syncSeederFile(SyncSeederListRequest msg);
  void connectToTracker();
  virtual ~TrackerServiceServer();
};

#endif