#include "TrackerServiceServer.h"
#include "encoder.h"
#include "networkInterfacer.h"
#include "utils.h"
#include "decoder.h"
#include <syslog.h>
#include <arpa/inet.h>
#include <string.h>
#include "clientDatabase.h"
#include "utils.h"

TrackerServiceServer::TrackerServiceServer()
{
    try
    {
        this->tracker_fd = createTCPClient(ClientDatabase::getInstance().getTracker1());
    }
    catch (std::exception e)
    {
        this->tracker_fd = createTCPClient(ClientDatabase::getInstance().getTracker2());
    }
}

Response TrackerServiceServer::shareFile(Share msg)
{
    Encoder encoder;
    auto b = encoder.encode("SHARE", msg.getBytes());
    NetworkWriter writer(this->tracker_fd);
    writer.writeToNetwork(b);

    NetworkReader reader(this->tracker_fd);
    auto response_b = reader.readFromNetwork();
    Decoder decoder;
    auto msg_pair = decoder.decodeMsgType(response_b);
    return Response(msg_pair.second);
}
Response TrackerServiceServer::addSeederRequest(AddSeeder msg)
{
    Encoder encoder;
    auto b = encoder.encode("ADDSEEDER", msg.getBytes());
    NetworkWriter writer(this->tracker_fd);
    writer.writeToNetwork(b);

    NetworkReader reader(this->tracker_fd);
    auto response_b = reader.readFromNetwork();
    Decoder decoder;
    auto msg_pair = decoder.decodeMsgType(response_b);
    return Response(msg_pair.second);
}

Response TrackerServiceServer::removeSeederRequest(RemoveSeeder msg)
{
    Encoder encoder;
    auto b = encoder.encode("REMOVESEEDER", msg.getBytes());
    NetworkWriter writer(this->tracker_fd);
    writer.writeToNetwork(b);

    NetworkReader reader(this->tracker_fd);
    auto response_b = reader.readFromNetwork();
    Decoder decoder;
    auto msg_pair = decoder.decodeMsgType(response_b);
    return Response(msg_pair.second);
}

SeederInfoResponse TrackerServiceServer::getSeederInfo(SeederInfoRequest msg)
{
    Encoder encoder;
    auto b = encoder.encode("SEEDERINFOREQUEST", msg.getBytes());
    NetworkWriter writer(this->tracker_fd);
    writer.writeToNetwork(b);

    std::cout << "Check" << std::endl;
    NetworkReader reader(this->tracker_fd);
    auto response_b = reader.readFromNetwork();
    std::cout << "Check2" << std::endl;
    Decoder decoder;
    auto msg_pair = decoder.decodeMsgType(response_b);
    std::cout << "Chec3" << std::endl;
    return SeederInfoResponse(msg_pair.second);
}
