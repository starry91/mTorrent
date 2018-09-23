#include "rpcHandler.h"
#include "networkInterfacer.h"
#include "decoder.h"
#include "trackerMessageHandler.h"
#include "message.h"
#include "encoder.h"

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
        //std::string request = msg->getType();
        TrackerMessageHandler msgHandler;
        NetworkWriter writer(client_fd);
        if (request == "SHARE")
        {
            std::cout << "hello1" << std::endl;
            auto res = msgHandler.handleShareRequest(byte_data);
            writer.writeToNetwork(encoder.encode(std::string("RESPONSE"),res.getBytes()));
        }
        else if (request == "ADDSEEDER")
        {
            std::cout << "hello2" << std::endl;
            auto res = msgHandler.handleAddSeederRequest(byte_data);
            writer.writeToNetwork(encoder.encode(std::string("RESPONSE"),res.getBytes()));
        }
        else if (request == "REMOVESEEDER")
        {
            std::cout << "hello3" << std::endl;
            auto res = msgHandler.handleRemoveSeederRequest(byte_data);
            writer.writeToNetwork(encoder.encode(std::string("RESPONSE"),res.getBytes()));
        }
        else if (request == "SEEDERINFOREQUEST")
        {
            std::cout << "hello4" << std::endl;
            auto res = msgHandler.handleGetSeedsRequest(byte_data);
            writer.writeToNetwork(encoder.encode(std::string("SEEDERINFORESPONSE"),res.getBytes()));
        }
    }
}