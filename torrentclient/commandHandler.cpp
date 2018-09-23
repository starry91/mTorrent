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

using std::cout;
using std::endl;

class ChunkSaver
{
    std::string filepath;
    std::vector<Seeder> seederlist;
    int chunkindex;
    std::string hash;

  public:
    ChunkSaver(std::string filepath, std::string hash, std::vector<Seeder> seeds, int chunkindex)
    {
        this->filepath = filepath;
        this->seederlist = seeds;
        this->chunkindex = chunkindex;
        this->hash = hash;
    }

    void downloadChunk()
    {
        std::fstream outfile;
        outfile.open(filepath);
        outfile.seekg(CHUNK_SIZE * chunkindex, std::ios::beg);
        TrackerServiceServer seeder(this->seederlist[0], this->seederlist[0]);
        SendChunkRequest req;
        req.setHash(hash);
        req.setChunkId(chunkindex);
        auto resp = seeder.getChunk(req);
        cout << "get chunk response for chunk: " << chunkindex << ", hash: " << resp.getHash() << ", size: " << resp.getChunkdata().size() << endl;
        outfile.write(resp.getChunkdata().data(), resp.getChunkdata().size());
        outfile.close();
    }
};

void CommandHandler::handleCommand(std::string command)
{
    cout << "in CommandHandler::handleCommand(), command: [" << command << "]" << endl;
    try
    {
        std::vector<std::string> args = extractArgs(command);
        if (args[0] == "share" && args.size() == 3)
        {
            FileHandler filehandler;

            std::cout << "args[1]: " << args[1] << "args[2]: " << args[2] << endl;

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
            cout << "CommandHandler::handleCommand() in download" << endl;
            FileHandler fhandler;
            std::cout << "CommandHandler::handleCommand() before Mtorr" << std::endl;
            auto mtorrPtr = fhandler.readMTorrent(args[1]);

            std::cout << "CommandHandler::handleCommand() read Mtorr" << std::endl;

            std::cout << "CommandHandler::handleCommand() Mtorr hash: " << mtorrPtr->getHash() << std::endl;
            std::cout << "CommandHandler::handleCommand() Mtorr file size: " << mtorrPtr->getFileSize() << std::endl;

            SeederInfoRequest req;
            req.setHash(mtorrPtr->getHash());

            //calling rpc
            try
            {
                TrackerServiceServer trackerCommunicator(ClientDatabase::getInstance().getTracker1(), ClientDatabase::getInstance().getTracker2());
                SeederInfoResponse res = trackerCommunicator.getSeederInfo(req);

                std::cout << "Seeder list size: " << res.getSeeders().size() << endl;

                std::map<std::string, ChunkInfoResponse> chunk_info_map;
                std::vector<std::vector<Seeder>> chunk_source(mtorrPtr->getBitChunks().size());

                for (auto s : res.getSeeders())
                {
                    TrackerServiceServer seeder(s, s);
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
                    chunk_info_map[s.getIp() + ":" + s.getPort()] = resp;
                }

                auto filename = mtorrPtr->getfileName().substr(mtorrPtr->getfileName().find_last_of("/"));
                auto destfilepath = args[2] + "/" + filename;
                std::fstream outfile;
                outfile.open(destfilepath, std::ios::trunc | std::ios::out | std::ios::binary);
                char buf[mtorrPtr->getFileSize()];
                outfile.write(buf, mtorrPtr->getFileSize());
                outfile.close();

                cout << "CommandHandler::handleCommand() File name: " << mtorrPtr->getfileName() << endl;

                std::shared_ptr<ChunkSaver> threads[chunk_source.size()];

                // download chunks
                for (int i = 0; i < chunk_source.size(); i++)
                {
                    ChunkSaver saver(destfilepath, mtorrPtr->getHash(), chunk_source[i], i);
                    saver.downloadChunk();
                }
            }
            catch (std::exception e)
            {
                std::cerr << "Unable to fetch seeder data" << endl;
            }
            //auto seeders = this->getSeeders(args[1]);
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