#include "AudiosurfMemoryScanner.h"
#include "../ASReader/MemoryReader.h"

AudiosurfMemoryScanner::AudiosurfMemoryScanner(QObject* parent)
    : QObject(parent), _selectedMode(ReadingMode::MemoryOffset)
{
    _engine = new MemoryReader();
}

AudiosurfMemoryScanner::~AudiosurfMemoryScanner()
{
}

AudiosurfData AudiosurfMemoryScanner::performFullScan() {
    return AudiosurfData();
}

AudiosurfData AudiosurfMemoryScanner::performFullScanMemOffsetImpl() {
    return AudiosurfData();
}

AudiosurfData AudiosurfMemoryScanner::performFullScanMemPatternsImpl() {
    throw LogicException("Called implementation unsupported or unimplemented");
}
