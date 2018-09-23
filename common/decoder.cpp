#include "decoder.h"
#include "message.h"
#include <utility>

using std::cout;
using std::endl;

std::pair<std::string, std::vector<char>> Decoder::decodeMsgType(std::vector<char> b)
{
    cout << "in Decoder::decodeMsgType() vsc size: " << b.size() << endl;
    uint32_t type_size = nvtouint32(std::vector<char>(&b[0], &b[4]));
    cout << "in Decoder::decodeMsgType() type size: " << type_size << endl;
    auto type = std::string(b.begin() + 4, b.begin() + 4 + type_size);
    cout << "in Decoder::decodeMsgType() type: " << type << endl;
    b.erase(b.begin(), b.begin() + 4 + type_size);
    cout << "in Decoder::decodeMsgType() vec after erase: " << b.size() << endl;

    return std::make_pair(type, b);
}