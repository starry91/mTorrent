#include "commandHandler.h"
#include "utils.h"
#include <syslog.h>
#include "TrackerServiceServer.h"
#include "mtorrent.h"
#include "clientDatabase.h"
#include "fileHandler.h"
#include "logHandler.h"
#include "errorMsg.h"

using std::cout;
using std::endl;

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

            //creating share msg for the rpc call
            Share msg;
            msg.setFileName(mtorr->getfileName());
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

            std::cout << "CommandHandler::handleCommand() Mtorr hash: " <<  mtorrPtr->getHash() << std::endl;
            std::cout << "CommandHandler::handleCommand() Mtorr file size: " <<  mtorrPtr->getFileSize() << std::endl;

            SeederInfoRequest req;
            req.setHash(mtorrPtr->getHash());

            //calling rpc
            try
            {
                TrackerServiceServer trackerCommunicator(ClientDatabase::getInstance().getTracker1(), ClientDatabase::getInstance().getTracker2());
                SeederInfoResponse res = trackerCommunicator.getSeederInfo(req);

                std::cout << "Seeder list size: " << res.getSeeders().size() << endl;
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