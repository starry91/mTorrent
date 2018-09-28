#include "encoder.h"
#include "utils.h"

std::vector<char> Encoder::encode(std::string type, std::vector<char> b)
{
    uint32_t size = type.size();
    auto buf = uint32tonv(size);
    buf.insert(buf.end(), type.begin(), type.end());
    std::cout << "In Encoder, before inserting empty vec: " << buf.size() << std::endl;
    buf.insert(buf.end(), b.begin(), b.end());
    std::cout << "In Encoder, after inserting empty vec: " << buf.size() << std::endl;
    return buf;
}