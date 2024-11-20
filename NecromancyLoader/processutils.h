#pragma once

#include "windefprettify.h"

class ProcessUtils final {
    ProcessUtils() = delete;
    ~ProcessUtils() = delete;

public:
    static WinDword getProcessId(const std::wstring& procName);
};
