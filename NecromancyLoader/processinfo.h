#pragma once

#include "propertymacro.h"
#include "windefprettify.h"

class ProcessInfo final : public QObject {
    Q_OBJECT

public:
    explicit ProcessInfo(QObject* parent = nullptr);
    virtual ~ProcessInfo() override = default;

    // C# moment lol
    AUTO_PROPERTY(WinDword, processId)
    AUTO_PROPERTY(QImage, icon)

    QString processName();
    void setProcessName(const QString& processName);

private:
    void loadProcessIcon();
    WinIconHandle getNativeProcIcon() const;

    QString _processName;
};

Q_DECLARE_METATYPE(ProcessInfo)
Q_DECLARE_METATYPE(ProcessInfo*)
