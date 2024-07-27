#include "pch.h"
#include "ChannelReader.h"

Necromancy::ChannelReader::ChannelReader() = default;

Necromancy::ChannelReader::ChannelReader(const std::string & chName) {
    _chGuid = chName;
}
