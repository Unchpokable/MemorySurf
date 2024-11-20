#pragma once

#include "windefprettify.h"
#include "propertymacro.h"

class ProcessInfo final : public QObject {
    Q_OBJECT

public:
    explicit ProcessInfo(QObject* parent = nullptr);
    ~ProcessInfo() = default;

    AUTO_PROPERTY(WinDword, processId)
    AUTO_PROPERTY(std::wstring, processName)
};
