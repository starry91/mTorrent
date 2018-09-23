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
                TrackerServiceServer trackerCommunicator(ClientDatabase::getInstance().getTracker1(), ClientDatabase::getInstance().getTracker2());
                Response res = trackerCommunicator.shareFile(msg);
                this->printResponse(msg.getType(), res);
            }
            catch (std::exception e)
            {
                Response res;
                res.setResponse("FAIL");
                this->printResponse(msg.getType(), res);
            }
        }
        else if (args[0] == "get")
        {
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