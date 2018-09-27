#include "rpcHandler.h"
#include "networkInterfacer.h"
#include "decoder.h"
#include "trackerMessageHandler.h"
#include "message.h"
#include "encoder.h"
#include "logHandler.h"
#include "errorMsg.h"
#include "trackerDatabase.h"
using std::cout;
using std::endl;

void RpcHandler::handleRpc(int client_fd)
{
    try
    {
        cout << "handleRpc() before while for fd: " << client_fd << endl;
        while (true)
        {
            NetworkReader reader(client_fd);
            auto byte_data = reader.readFromNetwork();
            std::cout << "byte_data size: " << byte_data.size() << std::endl;
            Decoder decoder;
            Encoder encoder;
            auto rpcbytepair = decoder.decodeMsgType(byte_data);
            auto request = rpcbytepair.first;
            byte_data = rpcbytepair.second;
            if (byte_data.size() == 0)
            {
                NetworkWriter writer(client_fd);
                Response res;
                res.setResponse("FAIL: Corrupted Data");
                writer.writeToNetwork(encoder.encode(std::string("RESPONSE"), res.getBytes()));
            }
            //std::string request = msg->getType();
            TrackerMessageHandler msgHandler;
            NetworkWriter writer(client_fd);
            cout << "handleRpc() request type: " << request << endl;
            if (request == "SHARE")
            {
                std::cout << "Recieved share request" << std::endl;
                LogHandler::getInstance().logMsg("Recieved share request");
                std::cout << "hello1" << std::endl;
                auto res = msgHandler.handleShareRequest(byte_data);
                writer.writeToNetwork(encoder.encode(std::string("RESPONSE"), res.getBytes()));
                if (res.getResponse() == "SUCCESS")
                {
                    TrackerDatabase::getInstance().updateSeederfile();
                }
            }
            else if (request == "ADDSEEDER")
            {
                std::cout << "Recieved AddSeeder request" << std::endl;
                LogHandler::getInstance().logMsg("Recieved AddSeeder request");
                std::cout << "hello2" << std::endl;
                auto res = msgHandler.handleAddSeederRequest(byte_data);
                writer.writeToNetwork(encoder.encode(std::string("RESPONSE"), res.getBytes()));
                if (res.getResponse() == "SUCCESS")
                {
                    TrackerDatabase::getInstance().updateSeederfile();
                }
            }
            else if (request == "REMOVESEEDER")
            {
                std::cout << "Recieved Remove Seeder request" << std::endl;
                LogHandler::getInstance().logMsg("Recieved Remove Seeder request");
                std::cout << "hello3" << std::endl;
                auto res = msgHandler.handleRemoveSeederRequest(byte_data);
                writer.writeToNetwork(encoder.encode(std::string("RESPONSE"), res.getBytes()));
                if (res.getResponse() == "SUCCESS")
                {
                    TrackerDatabase::getInstance().updateSeederfile();
                }
            }
            else if (request == "SEEDERINFOREQUEST")
            {
                std::cout << "Recieved Seeder Info request" << std::endl;
                LogHandler::getInstance().logMsg("Recieved SeederInfoRequest request");
                //std::cout << "handleRpc() in SEEDERINFOREQUEST before handleGetSeedsRequest" << std::endl;
                auto res = msgHandler.handleGetSeedsRequest(byte_data);
                //std::cout << "handleRpc() in SEEDERINFOREQUEST after handleGetSeedsRequest" << std::endl;
                writer.writeToNetwork(encoder.encode(std::string("SEEDERINFORESPONSE"), res.getBytes()));
                //std::cout << "handleRpc() in SEEDERINFOREQUEST after writing handleGetSeedsRequest" << std::endl;
            }
            else
            {
                NetworkWriter writer(client_fd);
                Response res;
                res.setResponse("FAIL: Invalid Request");
                writer.writeToNetwork(encoder.encode(std::string("RESPONSE"), res.getBytes()));
            }
        }
    }
    catch (ErrorMsg e)
    {
        std::cout << "Exception received: " << e.getErrorMsg() << "For fd: " << client_fd << std::endl;
    }
}