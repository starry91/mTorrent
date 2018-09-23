#include "rpcHandler.h"
#include "networkInterfacer.h"
#include "decoder.h"
#include "trackerMessageHandler.h"
#include "message.h"
#include "encoder.h"
#include "logHandler.h"

void RpcHandler::handleRpc(int client_fd)
{
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
        if (request == "SHARE")
        {
            LogHandler::getInstance().logMsg("Recieved share request");
            std::cout << "hello1" << std::endl;
            auto res = msgHandler.handleShareRequest(byte_data);
            writer.writeToNetwork(encoder.encode(std::string("RESPONSE"), res.getBytes()));
        }
        else if (request == "ADDSEEDER")
        {
            LogHandler::getInstance().logMsg("Recieved AddSeeder request");
            std::cout << "hello2" << std::endl;
            auto res = msgHandler.handleAddSeederRequest(byte_data);
            writer.writeToNetwork(encoder.encode(std::string("RESPONSE"), res.getBytes()));
        }
        else if (request == "REMOVESEEDER")
        {
            LogHandler::getInstance().logMsg("Recieved Remove Seeder request");
            std::cout << "hello3" << std::endl;
            auto res = msgHandler.handleRemoveSeederRequest(byte_data);
            writer.writeToNetwork(encoder.encode(std::string("RESPONSE"), res.getBytes()));
        }
        else if (request == "SEEDERINFOREQUEST")
        {
            LogHandler::getInstance().logMsg("Recieved SeederInfoRequest request");
            std::cout << "hello4" << std::endl;
            auto res = msgHandler.handleGetSeedsRequest(byte_data);
            writer.writeToNetwork(encoder.encode(std::string("SEEDERINFORESPONSE"), res.getBytes()));
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