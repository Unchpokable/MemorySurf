#pragma once

#include "windefprettify.h"

class ProcessInfo;

class ProcessUtils final {
    ProcessUtils() = delete;
    ~ProcessUtils() = delete;

public:
    static QList<ProcessInfo*> listActiveProcesses();

    static WinDword findProcessNamed(const QString &imageName);
};
