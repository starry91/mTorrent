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
        NetworkReader reader;
        auto byte_data = reader.readFromNetwork(client_fd);
        Decoder decoder;
        Encoder encoder;
        auto rpcbytepair = decoder.decodeMsgType(byte_data);
        auto request = rpcbytepair.first;
        byte_data = rpcbytepair.second;
        //std::string request = msg->getType();
        TrackerMessageHandler msgHandler;
        if (request == "SHARE")
        {
            std::cout << "hello1" << std::endl;
            auto res = msgHandler.handleShareRequest(byte_data);
            NetworkWriter writer;
            writer.writeToNetwork(client_fd, encoder.encode(std::string("RESPONSE"),res.getBytes()));
        }
        else if (request == "ADDSEEDER")
        {
            std::cout << "hello2" << std::endl;
            auto res = msgHandler.handleAddSeederRequest(byte_data);
            NetworkWriter writer;
            writer.writeToNetwork(client_fd, encoder.encode(std::string("RESPONSE"),res.getBytes()));
        }
        else if (request == "REMOVESEEDER")
        {
            std::cout << "hello3" << std::endl;
            auto res = msgHandler.handleRemoveSeederRequest(byte_data);
            NetworkWriter writer;
            writer.writeToNetwork(client_fd, encoder.encode(std::string("RESPONSE"),res.getBytes()));
        }
        else if (request == "GETSEEDS")
        {
            std::cout << "hello4" << std::endl;
            auto res = msgHandler.handleGetSeedsRequest(byte_data);
            NetworkWriter writer;
            writer.writeToNetwork(client_fd, encoder.encode(std::string("SEEDINFORESPONSE"),res.getBytes()));
        }
    }
}