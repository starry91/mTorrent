#include "trackerMessageHandler.h"
#include "decoder.h"
#include "message.h"
#include "trackerDatabase.h"
#include <syslog.h>

Response TrackerMessageHandler::handleShareRequest(std::vector<char> b)
{
    Decoder decoder;
    std::shared_ptr<Share> m = std::dynamic_pointer_cast<Share>(decoder.decode(b));
    auto &database = TrackerDatabase::getInstance();
    auto file_attr = std::make_shared<FileAttr>(FileAttr(m->getFileName(), m->getHash(), std::make_shared<Seeder>(Seeder(m->getIp(), m->getPort()))));
    database.addFileEntry(file_attr);
    Response res;
    res.setResponse("SUCCESS");
    return res;
}

Response TrackerMessageHandler::handleAddSeederRequest(std::vector<char> b)
{
    Decoder decoder;
    std::shared_ptr<AddSeeder> m = std::dynamic_pointer_cast<AddSeeder>(decoder.decode(b));
    auto seeder = std::make_shared<Seeder>(Seeder(m->getIp(), m->getPort()));
    auto &database = TrackerDatabase::getInstance();
    database.addSeeder(m->getHash(), seeder);
    Response res;
    res.setResponse("SUCCESS");
    return res;
}

Response TrackerMessageHandler::handleRemoveSeederRequest(std::vector<char> b)
{
    Decoder decoder;
    std::shared_ptr<RemoveSeeder> m = std::dynamic_pointer_cast<RemoveSeeder>(decoder.decode(b));
    auto seeder = std::make_shared<Seeder>(Seeder(m->getIp(), m->getPort()));
    auto &database = TrackerDatabase::getInstance();
    database.remove_seeder(m->getHash(), seeder);
    Response res;
    res.setResponse("SUCCESS");
    return res;
}

SeederInfoResponse TrackerMessageHandler::handleGetSeedsRequest(std::vector<char> b)
{
    Decoder decoder;
    std::shared_ptr<SeederInfoRequest> m = std::dynamic_pointer_cast<SeederInfoRequest>(decoder.decode(b));
    auto &database = TrackerDatabase::getInstance();
    SeederInfoResponse res;
    res.setHash(m->getHash());
    auto seeder_list = database.getSeederList(m->getHash());
    for (auto i : seeder_list)
    {
        res.addSeeder(*i);
    }
    return res;
}