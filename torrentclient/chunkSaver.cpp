#include "chunkSaver.h"
#include "fileHandler.h"
#include "utils.h"
#include <stdlib.h>
#include "download.h"
#include "downloadManager.h"

using std::cout;
using std::endl;

ChunkSaver::ChunkSaver(std::string filepath, std::string hash, std::vector<Seeder> seeds, int chunkindex)
{
    this->filepath = filepath;
    this->seederlist = seeds;
    this->chunkindex = chunkindex;
    this->hash = hash;
}

void ChunkSaver::downloadChunk()
{
    std::fstream outfile;
    outfile.open(filepath);
    outfile.seekg(CHUNK_SIZE * chunkindex, std::ios::beg);
    int index = rand()%this->seederlist.size();
    TrackerServiceServer seeder(this->seederlist[index]);
    SendChunkRequest req;
    req.setHash(hash);
    req.setChunkId(chunkindex);
    auto resp = seeder.getChunk(req);
    cout << "get chunk response for chunk: " << chunkindex << ", hash: " << resp.getHash() << ", size: " << resp.getChunkdata().size() << endl;
    outfile.write(resp.getChunkdata().data(), resp.getChunkdata().size());
    outfile.close();
}
