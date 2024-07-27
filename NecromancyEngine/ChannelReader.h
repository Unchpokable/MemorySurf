#pragma once

#include <string>

namespace Necromancy
{
    class ChannelReader;
}

class Necromancy::ChannelReader {
public:
    ChannelReader();
    explicit ChannelReader(const std::string& chName);

private:
    std::string _chGuid;
};

