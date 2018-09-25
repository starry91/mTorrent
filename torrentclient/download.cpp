#include "download.h"

Download::Download()
{
}
Download::Download(std::string hash, std::string file_name, std::string path, int tot_chunks)
{
    this->hash = hash;
    this->file_name = file_name;
    this->download_path = path;
    this->total_chunks = tot_chunks;
    this->downloadStatus = 0;
    this->downloaded_chunks = 0;
    this->chunk_map.resize(tot_chunks);
}
//setters
void Download::setHash(std::string)
{
}
void Download::setFileName(std::string)
{
}
void Download::setTotalChunks(int)
{
}

//getters
//setters
std::string Download::getHash()
{
    return this->hash;
}

std::string Download::getFileName()
{
    std::cout << "In Download class, getFileName|| [" << this->file_name << "]" << std::endl;
    return this->file_name;
}

int Download::getTotalChunks()
{
    return this->total_chunks;
}

void Download::updateChunkStatus(int index, int val)
{
    std::cout << "In Download class,updateChunkStatus|| Chunk Map size: " << this->chunk_map.size() << std::endl;
    this->chunk_map[index] = val;
}

void Download::incrementDownloadedChunks()
{
    this->downloaded_chunks += 1;
    if (this->downloaded_chunks == this->total_chunks)
    {
        this->downloadStatus = 1;
    }
}

int Download::getStatus()
{
    return this->downloadStatus;
}