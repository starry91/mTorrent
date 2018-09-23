#include "commandHandler.h"
#include "utils.h"
#include <syslog.h>
#include "TrackerServiceServer.h"
#include "mtorrent.h"
#include "clientDatabase.h"

using std::cout;
using std::endl;

void CommandHandler::handleCommand(std::string command)
{
    std::vector<std::string> args = extractArgs(command);
    if (args[0] == "share")
    {
        auto mtorr = std::make_shared<mTorrent>(args[1], args[2]);
        createMTorrent(mtorr);

        //creating share msg for the rpc call
        Share msg;
        msg.setFileName(mtorr->getfileName());
        msg.setHash(mtorr->getHash());
        msg.setIp(ClientDatabase::getInstance().getHost().getIp());
        msg.setPort(ClientDatabase::getInstance().getHost().getPort());

        //calling rpc
        try
        {
            TrackerServiceServer trackerCommunicator;
            Response res = trackerCommunicator.shareFile(msg);
            this->printResponse(res);
        }
        catch(std::exception e) 
        {
            Response res;
            res.setResponse("FAIL");
            this->printResponse(res);
        }
    }
    else if (args[0] == "get")
    {
        //auto seeders = this->getSeeders(args[1]);
    }
}

void CommandHandler::printResponse(Response res)
{
    cout << res.getResponse() << endl;
}