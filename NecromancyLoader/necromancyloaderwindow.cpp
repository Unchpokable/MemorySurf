#include "pch.h"
#include "necromancyloaderwindow.h"
#include "windefprettify.h"
#include "privileges.h"
#include "processinfo.h"

NecromancyLoaderWindow::NecromancyLoaderWindow(QWidget *parent)
        : QMainWindow(parent)
        , ui(new Ui::NecromancyLoaderWindowClass()) {
    ui->setupUi(this);
    setWindowTitle("Necromancy Loader");
    checkAndAdjustAppPrivileges();
}

NecromancyLoaderWindow::~NecromancyLoaderWindow() {
    delete ui;
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
