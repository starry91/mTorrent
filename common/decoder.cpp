#include "decoder.h"
#include "message.h"
#include <utility>

std::pair<std::string, std::vector<char>> Decoder::decodeMsgType(std::vector<char> b)
{
    uint32_t type_size = nvtouint32(std::vector<char>(&b[0], &b[4]));
    auto type = std::string(b.begin() + 4, b.begin() + 4 + type_size);
    b.erase(b.begin(), b.begin() + 4 + type_size);

    return std::make_pair(type, b);
}