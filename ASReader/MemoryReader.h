#pragma once

#include <optional>

#include "ASReader_globals.h"
#include "framework.h"
#include "traits.h"

class ASREADER_EXPORT MemoryReader {

public:
    using VoidPtr = LPCVOID;

    struct ByteArray
    {
        BYTE* Data;
        DWORD DataLength;
    };

    explicit MemoryReader(HANDLE target) : _targetProc(target) {}
    explicit MemoryReader(DWORD pid);

    MemoryReader(const MemoryReader&) = delete;
    MemoryReader(MemoryReader&&) = delete;
    MemoryReader& operator=(const MemoryReader&) = delete;
    MemoryReader& operator=(MemoryReader&&) = delete;

    ~MemoryReader()
    {
        if(_targetProc)
            CloseHandle(_targetProc);
    }

    ByteArray ReadMemory(VoidPtr address, DWORD size) const;

    ByteArray ReadMemoryPattern(BYTE* pattern, BYTE* mask, size_t patternSize);

    template<typename T, std::enable_if_t<is_simple_struct_v<T>, bool> = true>
    std::optional<T> ReadMemory(VoidPtr address) const;

    template<typename T, std::enable_if_t<is_simple_struct_v<T>, bool> = true>
    std::optional<T> ReadMemoryPattern(BYTE* pattern, BYTE* mask, size_t patternSize);

private:
    static bool CompareMemoryPattern(BYTE* data, BYTE* pattern, BYTE* mask, size_t patternSize);

    HANDLE _targetProc { nullptr };
};

template <typename T, std::enable_if_t<is_simple_struct_v<T>, bool> = true>
std::optional<T> MemoryReader::ReadMemory(VoidPtr address) const
{
    BYTE* buff = new BYTE[sizeof(T)];
    SIZE_T bytesRead;
    if(!ReadProcessMemory(_targetProc, address, buff, sizeof(T), &bytesRead) || bytesRead != sizeof(T))
    {
        delete buff;
        return std::nullopt;
    }
    auto data = reinterpret_cast<T*>(buff);
    return *data; // reinterpret read chunk as a given struct type and copy of its data 
}

template <typename T, std::enable_if_t<is_simple_struct_v<T>, bool>>
std::optional<T> MemoryReader::ReadMemoryPattern(BYTE* pattern, BYTE* mask, size_t patternSize)
{
    ByteArray rawData = ReadMemoryPattern(pattern, mask, patternSize);

    auto object = reinterpret_cast<T*>(rawData.Data);

    if(!object)
        return std::nullopt;

    return *object;
}
