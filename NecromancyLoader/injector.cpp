// ReSharper disable CppCStyleCast
// ReSharper disable CppZeroConstantCanBeReplacedWithNullptr
#include "pch.h"
#include "injector.h"
#include "processhelper.h"
#include "processutils.h"

Injector::Injector(QObject* parent) : QObject(parent), _targetProcId(0), _injectorHelper(new ProcessHelper(this))
{
    connect(_injectorHelper, &ProcessHelper::processFinished, this, &Injector::onInjectorExited);
}

void Injector::setTargetLibrary(const std::wstring& libraryName)
{
    _targetLibraryPath = libraryName;
}

void Injector::setTargetProcPid(WinDword procId)
{
    _targetProcId = procId;
}

void Injector::inject() const
{
    auto existingDll = findTargetDll();
    if(existingDll != NULL) {
        return;
    }

    _injectorHelper->setClArgs({ "-i", QString::number(_targetProcId), QString::fromStdWString(_targetLibraryPath) });
    _injectorHelper->setTargetProc("InjectHelper32.exe");
    _injectorHelper->run();
}

void Injector::free() const
{
    _injectorHelper->setClArgs({ "-u", QString::number(_targetProcId), QString::fromStdWString(_targetLibraryPath) });
    _injectorHelper->setTargetProc("InjectHelper32.exe");
    _injectorHelper->run();
}

bool Injector::hasTargetLoadedLibrary() const
{
    return findTargetDll() != NULL;
}

void Injector::onInjectorExited(int exitCode, const QString& stdOut)
{
    emit injectorExited(exitCode, stdOut);
}

WinModuleHandle Injector::findTargetDll() const
{
    WinHandle procHandle = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, _targetProcId);
    if(!procHandle) {
        // todo replace exceptions to status codes
        throw std::runtime_error("Denied access to process:");
    }

    WinModuleHandle moduleArray[1024];
    WinDword bytesRequired;

    if(!EnumProcessModulesEx(procHandle, moduleArray, sizeof(moduleArray), &bytesRequired, LIST_MODULES_32BIT)) {
        qDebug() << GetLastError();
        throw std::runtime_error("Unable to enumerate process modules");
    }

    size_t modulesCount = bytesRequired / sizeof(WinModuleHandle);

    for(size_t i { 0 }; i < modulesCount; i++) {
        wchar_t moduleName[MAX_PATH] = { 0 };

        if(GetModuleBaseNameW(procHandle, moduleArray[i], moduleName, MAX_PATH)) {
            if(_wcsicmp(_targetLibraryPath.c_str(), moduleName) == 0) {
                CloseHandle(procHandle);
                return moduleArray[i];
            }
        }
    }
    CloseHandle(procHandle);
    return NULL;
}
