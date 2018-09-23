#include "peerHandler.h"
#include "networkInterfacer.h"
#include "decoder.h"
#include "peerMessageHandler.h"
#include "message.h"
#include "encoder.h"
#include "logHandler.h"

void PeerHandler::handleRpc(int client_fd)
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
        // if (byte_data.size() == 0)
        // {
        // }
        //std::string request = msg->getType();
        PeerMessageHandler msgHandler;
        NetworkWriter writer(client_fd);
        if (request == "CHUNKINFOREQUEST")
        {
            LogHandler::getInstance().logMsg("Recieved ChunkInfoRequest request");
            std::cout << "hello1" << std::endl;
            auto res = msgHandler.handleChunkInfoRequest(byte_data);
            writer.writeToNetwork(encoder.encode(std::string("CHUNKINFORESPONSE"), res.getBytes()));
        }
        else if (request == "SENDCHUNKREQUEST")
        {
            LogHandler::getInstance().logMsg("Recieved Send Chunk request");
            std::cout << "hello2" << std::endl;
            auto res = msgHandler.handlesendChunkRequest(byte_data);
            writer.writeToNetwork(encoder.encode(std::string("SENDCHUNKRESPONSE"), res.getBytes()));
        }
    }
}