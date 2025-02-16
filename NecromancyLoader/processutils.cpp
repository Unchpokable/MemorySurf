#include "pch.h"
#include "processutils.h"
#include "processinfo.h"

QList<ProcessInfo*> ProcessUtils::listActiveProcesses() {
    QList<ProcessInfo*> processes;
    static const QStringList systemProcesses = {
        "system", "winlogon.exe", "csrss.exe", "svchost.exe", "services.exe"
    };

    WinHandle snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if(snapshot == INVALID_HANDLE_VALUE) {
        return processes;
    }

    ProcessEntry32 procEntry32;
    procEntry32.dwSize = sizeof(ProcessEntry32);

    if(Process32First(snapshot, &procEntry32)) {
        do {
            WinHandle proc = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, procEntry32.th32ProcessID);
            if(!proc)
                continue;

            WinDword sessionId = 0;
            if(ProcessIdToSessionId(procEntry32.th32ProcessID, &sessionId) && sessionId == 0) {
                CloseHandle(proc);
                continue;
            }

            char procPath[MAX_PATH] = { 0 };
            WinDword size(MAX_PATH);
            if(QueryFullProcessImageName(proc, 0, procPath, &size)) {
                wchar_t systemRootBuffer[MAX_PATH] = { 0 };
                size_t size = 0;
                _wgetenv_s(&size, systemRootBuffer, MAX_PATH, L"SystemRoot");

                QString systemRoot = QString::fromWCharArray(systemRootBuffer).replace("/", "\\");
                QString path = QString::fromLatin1(procPath);

                if(path.startsWith(systemRoot + "\\System32", Qt::CaseInsensitive) ||
                    path.startsWith(systemRoot + "\\SysWOW64", Qt::CaseInsensitive)) {
                    CloseHandle(proc);
                    continue;
                }
            }

            QString procName = QString::fromLatin1(procEntry32.szExeFile).toLower();
            if(systemProcesses.contains(procName)) {
                CloseHandle(proc);
                continue;
            }

            auto procInfo = new ProcessInfo();
            procInfo->set_processId(procEntry32.th32ProcessID);
            procInfo->setProcessName(procName);

            processes.append(procInfo);
        } while(Process32Next(snapshot, &procEntry32));
    }

    CloseHandle(snapshot);
    return processes;
}

WinDword ProcessUtils::findProcessNamed(const QString &imageName) {
    static const QStringList systemProcesses = {
        "system", "winlogon.exe", "csrss.exe", "svchost.exe", "services.exe"
    };

    WinHandle snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if(snapshot == INVALID_HANDLE_VALUE) {
        return 0;
    }

    PROCESSENTRY32W procEntry;
    procEntry.dwSize = sizeof(PROCESSENTRY32W);
    DWORD targetPid = 0;

    if(Process32FirstW(snapshot, &procEntry)) {
        do {
            WinHandle hProc = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, procEntry.th32ProcessID);
            if(!hProc) continue;

            DWORD sessionId = 0;
            if(ProcessIdToSessionId(procEntry.th32ProcessID, &sessionId) && sessionId == 0) {
                CloseHandle(hProc);
                continue;
            }

            WCHAR procPath[MAX_PATH] = { 0 };
            DWORD pathSize = MAX_PATH;
            if(QueryFullProcessImageNameW(hProc, 0, procPath, &pathSize)) {
                QString qProcPath = QString::fromWCharArray(procPath).replace("/", "\\").toLower();

                WCHAR systemRootEnv[MAX_PATH] = { 0 };
                size_t envSize = 0;
                _wgetenv_s(&envSize, systemRootEnv, L"SystemRoot");
                QString systemRoot = QString::fromWCharArray(systemRootEnv).replace("/", "\\").toLower();

                if(qProcPath.startsWith(systemRoot + "\\system32", Qt::CaseInsensitive) ||
                    qProcPath.startsWith(systemRoot + "\\syswow64", Qt::CaseInsensitive)) {
                    CloseHandle(hProc);
                    continue;
                }
            }

            QString procName = QString::fromWCharArray(procEntry.szExeFile).toLower();
            if(systemProcesses.contains(procName)) {
                CloseHandle(hProc);
                continue;
            }

            if(procName == imageName.toLower()) {
                targetPid = procEntry.th32ProcessID;
                CloseHandle(hProc);
                break;
            }

            CloseHandle(hProc);
        } while(Process32NextW(snapshot, &procEntry));
    }

    CloseHandle(snapshot);
    return targetPid;
}
