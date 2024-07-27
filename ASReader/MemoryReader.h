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

    enum class SearchMode
    {
        Absolute,
        Relative
    };

    MemoryReader() = default;
    explicit MemoryReader(HANDLE target) : _targetProc(target) {}
    explicit MemoryReader(DWORD pid);

    ~MemoryReader()
    {
        if(_targetProc)
            CloseHandle(_targetProc);
    }

    MemoryReader(const MemoryReader&) = delete;
    MemoryReader(MemoryReader&&) = delete;
    MemoryReader& operator=(const MemoryReader&) = delete;
    MemoryReader& operator=(MemoryReader&&) = delete;

    void setSearchModeToAbsolute()
    {
        _mode = SearchMode::Absolute;
    }

    void setSearchModeToRelative()
    {
        _mode = SearchMode::Relative;
    }

    void setTarget(DWORD pid);
    void setBaseModule(const WCHAR* moduleName);

    ByteArray readMemory(uintptr_t offset, DWORD size) const;

    ByteArray readMemoryPattern(BYTE* pattern, BYTE* mask, size_t patternSize) const;

    template<typename T, std::enable_if_t<is_simple_struct_v<T>, bool> = true>
    std::optional<T> readMemory(VoidPtr address) const;

    template<typename T, std::enable_if_t<is_simple_struct_v<T>, bool> = true>
    std::optional<T> readMemoryPattern(BYTE* pattern, BYTE* mask, size_t patternSize);

private:
    static bool compareMemoryPattern(BYTE* data, BYTE* pattern, BYTE* mask, size_t patternSize);

    DWORD _targetPid = 0;
    HANDLE _targetProc { nullptr };
    uintptr_t _baseAddress = 0;

    SearchMode _mode = SearchMode::Relative;
};

template <typename T, std::enable_if_t<is_simple_struct_v<T>, bool> = true>
std::optional<T> MemoryReader::readMemory(VoidPtr address) const
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
std::optional<T> MemoryReader::readMemoryPattern(BYTE* pattern, BYTE* mask, size_t patternSize)
{
    ByteArray rawData = readMemoryPattern(pattern, mask, patternSize);

    auto object = reinterpret_cast<T*>(rawData.Data);

    if(!object)
        return std::nullopt;

    return *object;
}
