#include "chunkSaver.h"
#include "fileHandler.h"
#include "utils.h"
#include <stdlib.h>
#include "download.h"
#include "downloadManager.h"
#include "utils.h"
#include "mtorrent.h"
#include "clientDatabase.h"

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
    int index = rand() % this->seederlist.size();
    std::cout << "Random index: " << index << std::endl;
    TrackerServiceServer seeder(this->seederlist[index]);
    SendChunkRequest req;
    req.setHash(hash);
    req.setChunkId(chunkindex);
    auto resp = seeder.getChunk(req);
    cout << "In chunk saver, get chunk response for chunk: " << chunkindex << ", hash: " << resp.getHash() << ", size: " << resp.getChunkdata().size() << endl;
    if (getChunkHash(resp.getChunkdata()) == hash.substr(20 * chunkindex, 20))
    {
        outfile.write(resp.getChunkdata().data(), resp.getChunkdata().size());
        DownloadManager::getInstance().updateFileChunkStatus(this->hash, this->chunkindex, 1);
        std::cout << "In Download chunk, After Updating fileChunkStatus" << std::endl;
        auto filename = this->filepath.substr(this->filepath.find_last_of("/"));
        std::cout << "In Download chunk, before creating mtorr" << std::endl;
        auto mTorr = std::make_shared<mTorrent>(this->filepath, filename);
        std::cout << "In Download chunk, after creating mtorr" << std::endl;
        mTorr->setHash(this->hash);
        mTorr->clearBitChunks();
        ClientDatabase::getInstance().addMTorrent(mTorr);
        ClientDatabase::getInstance().updateChunkInfo(this->hash, chunkindex, 1);
        DownloadManager::getInstance().addSeederRequestToTracker(this->hash);
    }
    outfile.close();
}
