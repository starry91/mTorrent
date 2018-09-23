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
    this->file_name;
}

int Download::getTotalChunks()
{
    return this->total_chunks;
}

void Download::updateChunkStatus(int index, int val)
{
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