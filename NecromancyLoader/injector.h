#pragma once

#include "windefprettify.h"
#include "processhelper.h"

class WinDllInjector final : public QObject {
    Q_OBJECT

public:
    Q_DISABLE_COPY_MOVE(WinDllInjector)

    WinDllInjector(QObject* parent);
    virtual ~WinDllInjector() override = default;

    void setTargetLibrary(const std::wstring& libraryName);
    void setTargetProcPid(WinDword procId);

    void inject() const;
    void free() const;
    bool hasTargetLoadedLibrary() const;

signals:
    void injectorExited(int exitCode, const QString& stdOut);

private slots:
    void onInjectorExited(int exitCode, const QString& stdOut);

private:
    WinModuleHandle findTargetDll() const;

    std::wstring _targetLibraryPath;
    WinDword _targetProcId;

    ProcessHelper* _injectorHelper;
};
