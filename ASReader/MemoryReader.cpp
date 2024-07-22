#include "pch.h"
#include "MemoryReader.h"

MemoryReader::MemoryReader(DWORD pid) {
    const auto proc = OpenProcess(PROCESS_VM_READ, FALSE, pid);
    if(!proc)
        throw std::runtime_error("Unable to open process");

    _targetProc = proc;
}

MemoryReader::ByteArray MemoryReader::ReadMemory(VoidPtr address, DWORD size) const
{
    ByteArray buffer;
    buffer.Data = new BYTE[size];

    SIZE_T bytesRead;

    if(!ReadProcessMemory(_targetProc, address, buffer.Data, size, &bytesRead) || bytesRead != size)
    {
        delete buffer.Data;
    }

    buffer.DataLength = size;
    return buffer;
}

MemoryReader::ByteArray MemoryReader::ReadMemoryPattern(BYTE * pattern, BYTE * mask, size_t patternSize) {
    ByteArray result;
    result.Data = new BYTE[patternSize];
    result.DataLength = patternSize;

    MEMORY_BASIC_INFORMATION mbi;
    SIZE_T bytesRead;
    BYTE* buffer = nullptr;
    BYTE* baseAddress = nullptr;

    while(VirtualQueryEx(_targetProc, baseAddress, &mbi, sizeof(mbi)) == sizeof(mbi))
    {
        if (mbi.State == MEM_COMMIT && (mbi.Protect & PAGE_READWRITE))
        {
            buffer = new BYTE[mbi.RegionSize];
            if (ReadProcessMemory(_targetProc, mbi.BaseAddress, buffer, mbi.RegionSize, &bytesRead))
            {
                for (size_t i {0ULL}; i < bytesRead - patternSize; i++)
                {
                    if (CompareMemoryPattern(buffer+i, pattern, mask, patternSize))
                    {
                        result.Data = buffer;
                        result.DataLength = patternSize;
                        return result;
                    }
                }
            }

            delete buffer;
        }

        baseAddress = (BYTE*)mbi.BaseAddress + mbi.RegionSize;
    }

    return result;
}

bool MemoryReader::CompareMemoryPattern(BYTE * data, BYTE * pattern, BYTE * mask, size_t patternSize) {
    for (size_t i {0ULL}; i < patternSize; i++)
    {
        if (mask[i] == 0x00)
        {
            continue;
        }
        if (data[i] != pattern[i])
        {
            return false;
        }
    }

    return true;
}
