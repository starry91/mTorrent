#include "encoder.h"
#include "utils.h"

std::vector<char> Encoder::encode(std::string type, std::vector<char> b)
{
    uint32_t size = type.size();
    auto buf = uint32tonv(size);
    buf.insert(buf.end(), type.begin(), type.end());
    buf.insert(buf.end(), b.begin(), b.end());
    return buf;
}