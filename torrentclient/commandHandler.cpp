#include "commandHandler.h"
#include "utils.h"
#include <syslog.h>
#include "TrackerServiceServer.h"
#include "mtorrent.h"
#include "clientDatabase.h"
#include "fileHandler.h"
#include "logHandler.h"
#include "errorMsg.h"
#include <map>
#include <queue>
#include <fstream>
#include "chunkSaver.h"
#include <thread>
#include "download.h"
#include "downloadManager.h"
#include <chrono>
#include "utils.h"
#include <thread>
#include "chunkDownloader.h"
using std::cout;
using std::endl;

void CommandHandler::handleCommand(std::string command)
{
    try
    {
        std::vector<std::string> args = extractArgs(command);
        if (args[0] == "share" && args.size() == 3)
        {
            //cout << "In share" << endl;
            FileHandler filehandler;
            auto mtorr = std::make_shared<mTorrent>(args[1], args[2]);
            filehandler.createMTorrent(mtorr);

            //updating database
            ClientDatabase::getInstance().addMTorrent(mtorr);

            //creating share msg for the rpc call
            Share msg;
            msg.setFileName(mtorr->getfileName());
            msg.setHash(mtorr->getHash());
            msg.setIp(ClientDatabase::getInstance().getHost().getIp());
            msg.setPort(ClientDatabase::getInstance().getHost().getPort());

            //calling rpc
            try
            {
                //std::cout << "handleCommand() sending msg" << std::endl;
                TrackerServiceServer trackerCommunicator(ClientDatabase::getInstance().getTracker1(), ClientDatabase::getInstance().getTracker2());
                Response res = trackerCommunicator.shareFile(msg);
                this->printResponse(msg.getType(), res);
                //std::cout << "handleCommand() Got reponse" << std::endl;
            }
            catch (std::exception e)
            {
                Response res;
                res.setResponse("FAIL");
                this->printResponse(msg.getType(), res);
            }
        }
        else if (args[0] == "download" && args.size() == 3)
        {
            FileHandler fhandler;
            auto mtorrPtr = fhandler.readMTorrent(args[1]);
            //cout << "Int download, after reading mTorrent: " << mtorrPtr->getfileName() << endl;
            // std::cout << "CommandHandler::handleCommand() read Mtorr" << std::endl;

            // std::cout << "CommandHandler::handleCommand() Mtorr hash: " << mtorrPtr->getHash() << std::endl;
            // std::cout << "CommandHandler::handleCommand() Mtorr file size: " << mtorrPtr->getFileSize() << std::endl;

            SeederInfoRequest req;
            req.setHash(mtorrPtr->getHash());

            //calling rpc
            try
            {
                TrackerServiceServer trackerCommunicator(ClientDatabase::getInstance().getTracker1(), ClientDatabase::getInstance().getTracker2());
                SeederInfoResponse res = trackerCommunicator.getSeederInfo(req);

                //std::cout << "Seeder list size: " << res.getSeeders().size() << endl;

                //std::map<std::string, ChunkInfoResponse> chunk_info_map;
                if (res.getStatus() == "SUCCESS")
                {
                    std::vector<std::vector<Seeder>> chunk_source(mtorrPtr->getBitChunks().size());

                    for (auto s : res.getSeeders())
                    {
                        TrackerServiceServer seeder(s);
                        ChunkInfoRequest req;
                        req.setHash(mtorrPtr->getHash());
                        auto resp = seeder.getChunkInfo(req);
                        //cout << "Chunk info response: " << resp.getHash() << endl;
                        for (int i = 0; i < resp.getChunkInfo().size(); i++)
                        {
                            //cout << "Chunk " << i << ", value: " << resp.getChunkInfo()[i] << endl;
                            if (resp.getChunkInfo()[i] == 1
                            && (s.getIp() != ClientDatabase::getInstance().getHost().getPort()
                                || s.getPort() != ClientDatabase::getInstance().getHost().getPort())
                            )
                            {
                                chunk_source[i].push_back(s);
                            }
                        }
                        //chunk_info_map[s.getIp() + ":" + s.getPort()] = resp;
                    }

                    //create empty file
                    // auto filename = mtorrPtr->getfileName().substr(mtorrPtr->getfileName().find_last_of("/") + 1);
                    // auto destfilepath = args[2] + "/" + filename;
                    auto destfilepath = args[2] + "/" + mtorrPtr->getfileName();
                    // cout << "In downoad, destfilepath: " << destfilepath << endl;
                    fhandler.createEmptyFile(destfilepath, mtorrPtr->getFileSize());

                    // cout << "Int download, after creating empty file: " << destfilepath << endl;
                    //cout << "CommandHandler::handleCommand() File name:    " << mtorrPtr->getfileName() << endl;

                    //std::shared_ptr<ChunkSaver> threads[chunk_source.size()];

                    std::vector<std::thread> thread_arr;
                    down_Sptr dPtr = std::make_shared<Download>(Download(mtorrPtr->getHash(), mtorrPtr->getfileName(), destfilepath, chunk_source.size()));
                    //std::cout << "Total chunks to be downloaded: " << dPtr->getTotalChunks() << std::endl;
                    DownloadManager::getInstance().addFile(dPtr);
                    ChunkDownloader chunkdownldr;
                    // download chunks
                    for (int i = 0; i < chunk_source.size(); i++)
                    {
                        //thread_arr.push_back(std::thread(&ChunkSaver::downloadChunk, ChunkSaver(destfilepath, mtorrPtr->getHash(), chunk_source[i], i)));
                        // cout << "Thread index: " << i << endl;
                        //std::this_thread::sleep_for(std::chrono::milliseconds(4000));
                        chunkdownldr.add(ChunkSaver(destfilepath, mtorrPtr->getHash(), chunk_source[i], i));
                    }
                    //waiting to get a signal from atleast 1 thread to update the tracker about seeding the file
                    for (int i = 0; i < 8; i++)
                    {
                        thread_arr.push_back(std::thread(&ChunkDownloader::downloadChunks, std::ref(chunkdownldr)));
                    }
                    for (auto &i : thread_arr)
                    {
                        if (i.joinable())
                            i.join();
                    }
                    std::cout << "Download Complete" << endl;
                }
                else
                {
                    throw ErrorMsg("No seeders exists");
                }
            }
            catch (std::exception e)
            {
                std::cerr << "Unable to fetch seeder data" << endl;
            }
            //auto seeders = this->getSeeders(args[1]);
        }
        else if (args[0] == "show" && args.size() == 2 && args[1] == "downloads")
        {
            DownloadManager::getInstance().printDownloads();
        }
        else if (args[0] == "remove" && args.size() == 2)
        {
            FileHandler fhandler;
            auto mtorrPtr = fhandler.readMTorrent(args[1]);
            if (DownloadManager::getInstance().hasDownloaded(mtorrPtr->getHash()))
            {
                int status = DownloadManager::getInstance().getDownloadStatus(mtorrPtr->getHash());
                //cout << "In command handler, getting file status: " << status << std::endl;
                if (status == 1)
                {
                    RemoveSeeder req;
                    req.setHash(mtorrPtr->getHash());
                    req.setIp(ClientDatabase::getInstance().getHost().getIp());
                    req.setPort(ClientDatabase::getInstance().getHost().getPort());
                    //cout << "In command handler, sending remove seeder req: " << status << std::endl;
                    TrackerServiceServer trackerCommunicator(ClientDatabase::getInstance().getTracker1(), ClientDatabase::getInstance().getTracker2());
                    Response res = trackerCommunicator.removeSeederRequest(req);
                    if (res.getResponse() == "SUCCESS")
                    {
                        ClientDatabase::getInstance().removeMTorrent(mtorrPtr->getHash());
                        cout << "SUCCESS" << endl;
                    }
                }
            }
            else
            {
                RemoveSeeder req;
                req.setHash(mtorrPtr->getHash());
                req.setIp(ClientDatabase::getInstance().getHost().getIp());
                req.setPort(ClientDatabase::getInstance().getHost().getPort());
                TrackerServiceServer trackerCommunicator(ClientDatabase::getInstance().getTracker1(), ClientDatabase::getInstance().getTracker2());
                Response res = trackerCommunicator.removeSeederRequest(req);
                if (res.getResponse() == "SUCCESS")
                {
                    ClientDatabase::getInstance().removeMTorrent(mtorrPtr->getHash());
                    cout << "SUCCESS" << endl;
                }
            }
        }
        else
        {
            std::cerr << "Invalid Command" << endl;
        }
    }
    catch (ErrorMsg m)
    {
        LogHandler::getInstance().logError(m.getErrorMsg());
        this->printError(m.getErrorMsg());
    }
}

void CommandHandler::printResponse(std::string msg, Response res)
{
    LogHandler::getInstance().logMsg("Request: " + msg);
    LogHandler::getInstance().logMsg("Response: " + res.getResponse());
    cout << res.getResponse() << endl;
}

void CommandHandler::printResponse(Response res)
{
    LogHandler::getInstance().logMsg(res.getResponse());
    cout << res.getResponse() << endl;
}

void CommandHandler::printError(std::string e)
{
    LogHandler::getInstance().logMsg(e);
    cout << e << endl;
}