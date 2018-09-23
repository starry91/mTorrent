#include "fileHandler.h"
#include "clientDatabase.h"
#include <fstream>
#include <syslog.h>
#include <math.h>
#include "mtorrent.h"
#include "seeder.h"
#include "openssl/sha.h"
#include "errorMsg.h"

//generating mtorrent file
void FileHandler::createMTorrent(mTorrent_Sptr torr)
{
    std::ofstream myfile;
    myfile.open(torr->getfileName().c_str());
    myfile.exceptions(std::ios::badbit);
    if (myfile.bad())
    {
        throw ErrorMsg("Invalid torrent path");
    }
    if (myfile.is_open())
    {
        syslog(0, "Writing file to: [%s]", torr->getPath().c_str());
        myfile << torr->getHash() << "\n";
        myfile << torr->getPath() << "\n";
        myfile << torr->getFileSize() << "\n";
        myfile << ClientDatabase::getInstance().getTracker1().getIp() << "\n";
        myfile << ClientDatabase::getInstance().getTracker1().getPort() << "\n";
        myfile << ClientDatabase::getInstance().getTracker2().getIp() << "\n";
        myfile << ClientDatabase::getInstance().getTracker2().getPort() << "\n";
    }
    myfile.close();
}

//getting the hash of the file
std::string FileHandler::getFileHash(std::string file_name)
{
    std::ifstream file;
    file.open(file_name, std::ios::in | std::ios::binary);
    //myfile.exceptions(std::ios::badbit);
    if (!file.good())
    {
        throw ErrorMsg("File does not exists");
    }
    file.ignore(std::numeric_limits<std::streamsize>::max());
    std::streamsize file_size = file.gcount();
    std::string hash = "";
    int chunk_count = ceil((file_size * 1.00) / CHUNK_SIZE);
    for (int i = 0; i < chunk_count; i++)
    {
        char data[CHUNK_SIZE + 1];
        file.seekg(i * CHUNK_SIZE);
        size_t length;
        if (i == chunk_count - 1)
        {
            file.read(data, file_size % CHUNK_SIZE);
            data[file_size % CHUNK_SIZE] = '\0';
            length = file_size % CHUNK_SIZE;
        }
        else
        {
            file.read(data, CHUNK_SIZE);
            data[CHUNK_SIZE] = '\0';
            length = CHUNK_SIZE;
        }

        unsigned char hash_buff[SHA_DIGEST_LENGTH];
        SHA1(reinterpret_cast<const unsigned char *>(data), length, hash_buff);

        char mdString[SHA_DIGEST_LENGTH * 2];

        for (int i = 0; i < SHA_DIGEST_LENGTH; i++)
        {
            sprintf(&mdString[i * 2], "%02x", (unsigned int)hash_buff[i]);
        }
        std::string chunk_hash = std::string((char *)mdString);
        hash += chunk_hash.substr(0, 20);
        syslog(0, "Chunk Hash: [%s]", chunk_hash.c_str());
    }
    return hash;
}

long FileHandler::fileSize(std::string path)
{
    std::ifstream file;
    file.open(path, std::ios::in | std::ios::binary);
    if (!file.good())
    {
        throw ErrorMsg("File does not exists");
    }
    file.ignore(std::numeric_limits<std::streamsize>::max());
    std::streamsize file_size = file.gcount();
    file.close();
    return file_size;
}

void FileHandler::readFileChunk(int chunk_index, std::string path, std::vector<char> &buffer)
{
    std::ifstream file;
    file.open(path, std::ios::in | std::ios::binary);
    if (!file.good())
    {
        throw ErrorMsg("File does not exists");
    }
    file.seekg(CHUNK_SIZE*chunk_index);
    //cut it off to available bytes
      // create a buffer
    file.read(&buffer[0], buffer.size()); // read to buffer
    file.close();
}