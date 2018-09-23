#include "trackerMessageHandler.h"
#include "decoder.h"
#include "message.h"
#include "trackerDatabase.h"
#include <syslog.h>

using std::cout;
using std::endl;

Response TrackerMessageHandler::handleShareRequest(std::vector<char> b)
{
    Response res;
    try
    {
        Decoder decoder;
        std::shared_ptr<Share> m = std::make_shared<Share>(b);
        auto &database = TrackerDatabase::getInstance();
        auto file_attr = std::make_shared<FileAttr>(FileAttr(m->getFileName(), m->getHash(), std::make_shared<Seeder>(Seeder(m->getIp(), m->getPort()))));
        database.addFileEntry(file_attr);
    }
    catch (...)
    {
        res.setResponse("FAIL");
        return res;
    }
    res.setResponse("SUCCESS");
    return res;
}

Response TrackerMessageHandler::handleAddSeederRequest(std::vector<char> b)
{
    Response res;
    try
    {
        Decoder decoder;
        std::shared_ptr<AddSeeder> m = std::make_shared<AddSeeder>(b);
        auto seeder = std::make_shared<Seeder>(Seeder(m->getIp(), m->getPort()));
        auto &database = TrackerDatabase::getInstance();
        database.addSeeder(m->getHash(), seeder);
    }
    catch (...)
    {
        std::cout << "fail" << std::endl;
        res.setResponse("FAIL");
        return res;
    }
    res.setResponse("SUCCESS");
    return res;
}

Response TrackerMessageHandler::handleRemoveSeederRequest(std::vector<char> b)
{
    Response res;
    try
    {
        Decoder decoder;
        std::shared_ptr<RemoveSeeder> m = std::make_shared<RemoveSeeder>(b);
        auto seeder = std::make_shared<Seeder>(Seeder(m->getIp(), m->getPort()));
        auto &database = TrackerDatabase::getInstance();
        database.remove_seeder(m->getHash(), seeder);
    }
    catch (...)
    {
        res.setResponse("FAIL");
        return res;
    }
    res.setResponse("SUCCESS");
    return res;
}

SeederInfoResponse TrackerMessageHandler::handleGetSeedsRequest(std::vector<char> b)
{
    SeederInfoResponse res;
    Decoder decoder;
    std::shared_ptr<SeederInfoRequest> m = std::make_shared<SeederInfoRequest>(b);
    res.setHash(m->getHash());
    try
    {
        auto &database = TrackerDatabase::getInstance();
        auto seeder_list = database.getSeederList(m->getHash());
        for (auto i : seeder_list)
        {
            cout<< "handleGetSeedsRequest() adding seeder: " << i->getIp() << endl;
            res.addSeeder(*i);
        }
        res.setStatus("SUCCESS");
        return res;
    }
    catch (...)
    {
        res.addSeeder(Seeder("0.0.0.0","0"));
        res.setStatus("FAIL");
        return res;
    }
}