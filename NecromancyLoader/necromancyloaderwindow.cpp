#include "pch.h"
#include "necromancyloaderwindow.h"
#include "privileges.h"
#include "processinfo.h"
#include "windefprettify.h"

NecromancyLoaderWindow::NecromancyLoaderWindow(QWidget *parent)
        : QMainWindow(parent)
        , ui(new Ui::NecromancyLoaderWindowClass()) {
    ui->setupUi(this);

    qRegisterMetaType<ProcessInfo>();
    qRegisterMetaType<ProcessInfo*>();

    setWindowTitle("Necromancy Loader");
    checkAndAdjustAppPrivileges();

    scanProcessesAndPopulateSelectionCombo();

    ui->statusBar->showMessage("Server stopped; Game offline; Game service not loaded");
}

NecromancyLoaderWindow::~NecromancyLoaderWindow() {
    delete ui;
}

void NecromancyLoaderWindow::scanProcessesAndPopulateSelectionCombo() {
    auto scannedProcesses = listActiveProcesses();

    for(auto proc : scannedProcesses) {
        // todo: default icons for unregistered application types
        auto iconPixmap = QPixmap::fromImage(proc->icon());
        QIcon icon(iconPixmap);

        ui->gameProcCombo->addItem(icon, proc->processName(), QVariant::fromValue(proc));
    }
    //todo auto select item with game process if exists
}

void NecromancyLoaderWindow::checkAndAdjustAppPrivileges() {
    WinHandle token;
    if(!OpenProcessToken(GetCurrentProcess(), TOKEN_QUERY, &token)) {
        QMessageBox::warning(this, "Application rights warning", "You launched NecromancyLoader not as admin. "
                                        "This may cause some errors, instability or total malfunctioning of software. Please, re-run application as admin");
        return;
    }

    TOKEN_ELEVATION elevation;
    WinDword size;
    if(!GetTokenInformation(token, TokenElevation, &elevation, sizeof(elevation), &size)) {
        CloseHandle(token);
        QMessageBox::warning(this, "Application rights warning", "You launched NecromancyLoader not as admin. "
            "This may cause some errors, instability or total malfunctioning of software. Please, re-run application as admin");
        return;
    }

    CloseHandle(token);
    if(elevation.TokenIsElevated == 0) {
        QMessageBox::warning(this, "Application rights warning", "You launched NecromancyLoader not as admin. "
            "This may cause some errors, instability or total malfunctioning of software. Please, re-run application as admin");
        return;
    }
    auto result = QMessageBox::question(this, "Application privileges", "NecromancyLoader has a highly close interactions with a game process."
        "To do this this application can use a privileges of debug application to improve interaction quality. Would you like to expand privileges to external debugger level?");
    if(result == QMessageBox::Yes) {
        RaisePrivilegesToDebug();
    }
}

QList<ProcessInfo *> NecromancyLoaderWindow::listActiveProcesses(){
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

            wchar_t procPath[MAX_PATH] = { 0 };
            WinDword size(MAX_PATH);
            if(QueryFullProcessImageName(proc, 0, procPath, &size)) {
                wchar_t systemRootBuffer[MAX_PATH] = { 0 };
                size_t size = 0;
                _wgetenv_s(&size, systemRootBuffer, MAX_PATH, L"SystemRoot");

                QString systemRoot = QString::fromWCharArray(systemRootBuffer).replace("/", "\\");
                QString path = QString::fromWCharArray(procPath);

                if(path.startsWith(systemRoot + "\\System32", Qt::CaseInsensitive) ||
                    path.startsWith(systemRoot + "\\SysWOW64", Qt::CaseInsensitive)) {
                    CloseHandle(proc);
                    continue;
                }
            }

            QString procName = QString::fromWCharArray(procEntry32.szExeFile).toLower();
            if(systemProcesses.contains(procName)) {
                CloseHandle(proc);
                continue;
            }

            auto procInfo = new ProcessInfo(this);
            procInfo->set_processId(procEntry32.th32ProcessID);
            procInfo->setProcessName(procName);

            processes.append(procInfo);
        } while(Process32Next(snapshot, &procEntry32));
    }

    CloseHandle(snapshot);
    return processes;
}
