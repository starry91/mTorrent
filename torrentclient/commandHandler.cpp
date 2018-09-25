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
#include <thread>
using std::cout;
using std::endl;

void CommandHandler::handleCommand(std::string command)
{
    // cout << "in CommandHandler::handleCommand(), command: [" << command << "]" << endl;
    try
    {
        std::vector<std::string> args = extractArgs(command);
        if (args[0] == "share" && args.size() == 3)
        {
            FileHandler filehandler;

            // std::cout << "args[1]: " << args[1] << "args[2]: " << args[2] << endl;

            auto mtorr = std::make_shared<mTorrent>(args[1], args[2]);
            filehandler.createMTorrent(mtorr);

            //updating database
            ClientDatabase::getInstance().addMTorrent(mtorr);

            auto filename = mtorr->getfileName().substr(mtorr->getfileName().find_last_of("/"));

            //creating share msg for the rpc call
            Share msg;
            msg.setFileName(filename);
            msg.setHash(mtorr->getHash());
            msg.setIp(ClientDatabase::getInstance().getHost().getIp());
            msg.setPort(ClientDatabase::getInstance().getHost().getPort());

            //calling rpc
            try
            {
                std::cout << "handleCommand() sending msg" << std::endl;
                TrackerServiceServer trackerCommunicator(ClientDatabase::getInstance().getTracker1(), ClientDatabase::getInstance().getTracker2());
                Response res = trackerCommunicator.shareFile(msg);
                this->printResponse(msg.getType(), res);
                std::cout << "handleCommand() Got reponse" << std::endl;
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
            // cout << "CommandHandler::handleCommand() in download" << endl;
            FileHandler fhandler;
            // std::cout << "CommandHandler::handleCommand() before Mtorr" << std::endl;
            auto mtorrPtr = fhandler.readMTorrent(args[1]);

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
                std::vector<std::vector<Seeder>> chunk_source(mtorrPtr->getBitChunks().size());

                for (auto s : res.getSeeders())
                {
                    TrackerServiceServer seeder(s);
                    ChunkInfoRequest req;
                    req.setHash(mtorrPtr->getHash());
                    auto resp = seeder.getChunkInfo(req);
                    cout << "Chunk info response: " << resp.getHash() << endl;
                    for (int i = 0; i < resp.getChunkInfo().size(); i++)
                    {
                        cout << "Chunk " << i << ", value: " << resp.getChunkInfo()[i] << endl;
                        if (resp.getChunkInfo()[i] == 1)
                        {
                            chunk_source[i].push_back(s);
                        }
                    }
                    //chunk_info_map[s.getIp() + ":" + s.getPort()] = resp;
                }

                //create empty file
                auto filename = mtorrPtr->getfileName().substr(mtorrPtr->getfileName().find_last_of("/"));
                auto destfilepath = args[2] + "/" + filename;
                fhandler.createEmptyFile(destfilepath, mtorrPtr->getFileSize());

                //cout << "CommandHandler::handleCommand() File name: " << mtorrPtr->getfileName() << endl;

                //std::shared_ptr<ChunkSaver> threads[chunk_source.size()];

                std::vector<std::thread> thread_arr;
                down_Sptr dPtr = std::make_shared<Download>(Download(mtorrPtr->getHash(), filename, destfilepath, chunk_source.size()));
                std::cout << "Total chunks to be downloaded: " << dPtr->getTotalChunks() << std::endl;
                DownloadManager::getInstance().addFile(dPtr);
                // download chunks
                for (int i = 0; i < chunk_source.size(); i++)
                {
                    thread_arr.push_back(std::thread(&ChunkSaver::downloadChunk, ChunkSaver(destfilepath, mtorrPtr->getHash(), chunk_source[i], i)));
                    //std::this_thread::sleep_for(std::chrono::milliseconds(4000));
                }
                //waiting to get a signal from atleast 1 thread to update the tracker about seeding the file
                {
                }
                for (auto &i : thread_arr)
                {
                    if (i.joinable())
                        i.join();
                }
                std::cout << "Download Complete" << endl;
            }
            catch (std::exception e)
            {
                std::cerr << "Unable to fetch seeder data" << endl;
            }
            //auto seeders = this->getSeeders(args[1]);
        }
        else if (args[0] == "show" && args.size() == 2)
        {
            DownloadManager::getInstance().printDownloads();
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