#include "pch.h"
#include "MemoryReader.h"

MemoryReader::MemoryReader(DWORD pid) : _targetPid(pid)
{
    setTarget(pid);
}

void MemoryReader::setTarget(DWORD pid)
{
    const auto proc = OpenProcess(PROCESS_VM_READ, FALSE, pid);
    if (!proc)
        throw std::runtime_error("Unable to open process");

    _targetProc = proc;
}

void MemoryReader::setBaseModule(const WCHAR* moduleName)
{
    _mode = SearchMode::Relative;

    uintptr_t moduleBaseAddr = 0;

    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, _targetPid);

    if (hSnapshot == INVALID_HANDLE_VALUE)
        throw std::runtime_error("Failed to create snapshot of the process modules.");

    MODULEENTRY32W moduleEntry32;
    moduleEntry32.dwSize = sizeof(MODULEENTRY32W);

    if (Module32FirstW(hSnapshot, &moduleEntry32))
    {
        do
        {
            if (wcscmp(moduleEntry32.szModule, moduleName) == 0)
            {
                moduleBaseAddr = reinterpret_cast<uintptr_t>(moduleEntry32.modBaseAddr);
                break;
            }
        }
        while (Module32NextW(hSnapshot, &moduleEntry32));
    }

    CloseHandle(hSnapshot);
    _baseAddress = moduleBaseAddr;
}

MemoryReader::ByteArray MemoryReader::readMemory(uintptr_t offset, DWORD size) const
{
    uintptr_t address = 0;

    if (_mode == SearchMode::Relative)
    {
        address += _baseAddress;
    }

    ByteArray buffer;
    buffer.Data = new BYTE[size];

    SIZE_T bytesRead;

    if (!ReadProcessMemory(_targetProc, reinterpret_cast<VoidPtr>(address + offset), buffer.Data, size, &bytesRead) ||
        bytesRead != size) // NOLINT
    {
        delete buffer.Data;
    }

    buffer.DataLength = size;
    return buffer;
}

MemoryReader::ByteArray MemoryReader::readMemoryPattern(BYTE* pattern, BYTE* mask, size_t patternSize) const
{
    ByteArray result;
    result.Data = new BYTE[patternSize];
    result.DataLength = patternSize; // NOLINT

    MEMORY_BASIC_INFORMATION mbi;
    SIZE_T bytesRead;
    BYTE* buffer = nullptr;
    BYTE* baseAddress = nullptr;

    while (VirtualQueryEx(_targetProc, baseAddress, &mbi, sizeof(mbi)) == sizeof(mbi))
    {
        if (mbi.State == MEM_COMMIT && (mbi.Protect & PAGE_READWRITE))
        {
            buffer = new BYTE[mbi.RegionSize];
            if (ReadProcessMemory(_targetProc, mbi.BaseAddress, buffer, mbi.RegionSize, &bytesRead))
            {
                for (size_t i{0ULL}; i < bytesRead - patternSize; i++)
                {
                    if (compareMemoryPattern(buffer + i, pattern, mask, patternSize))
                    {
                        result.Data = buffer;
                        result.DataLength = patternSize; // NOLINT
                        return result;
                    }
                }
            }

            delete[] buffer;
        }

        baseAddress = static_cast<BYTE*>(mbi.BaseAddress) + mbi.RegionSize;
    }

    return result;
}

bool MemoryReader::compareMemoryPattern(BYTE* data, BYTE* pattern, BYTE* mask, size_t patternSize)
{
    for (size_t i{0ULL}; i < patternSize; i++)
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
