#include "pch.h"
#include "processinfo.h"

#include "processutils.h"

ProcessInfo::ProcessInfo(QObject *parent) : QObject(parent), _processId(0) {
    // empty
}

QString ProcessInfo::processName() {
    return _processName;
}

void ProcessInfo::setProcessName(const QString& processName) {
    _processName = processName;
    loadProcessIcon();
}

void ProcessInfo::loadProcessIcon() {
    auto nativeIcon = getNativeProcIcon();
    if(nativeIcon == NULL) {
        return;
    }

    set_icon(QImage::fromHICON(nativeIcon));
}

WinIconHandle ProcessInfo::getNativeProcIcon() const {
    if(_processName.isEmpty()) {
        return NULL;
    }

    WinDword procId = _processId;
    WinHandle procHandle = OpenProcess(PROCESS_QUERY_LIMITED_INFORMATION, FALSE, procId);

    char path[MAX_PATH];
    WinDword size = MAX_PATH;
    if(QueryFullProcessImageName(procHandle, 0, path, &size)) {
        CloseHandle(procHandle);

        return ExtractIcon(nullptr, path, 0);
    }

    return NULL;
}
