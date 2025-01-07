#include "pch.h"
#include "necromancyloaderwindow.h"
#include "privileges.h"
#include "processinfo.h"
#include "windefprettify.h"
#include "injector.h"
#include "processutils.h"

NecromancyLoaderWindow::NecromancyLoaderWindow(QWidget *parent)
        : QMainWindow(parent), _injector(new WinDllInjector(this)), ui(new Ui::NecromancyLoaderWindowClass()) {
    ui->setupUi(this);

    qRegisterMetaType<ProcessInfo>();
    qRegisterMetaType<ProcessInfo*>();

    loadProperties();

    setWindowTitle("Necromancy Loader");
    checkAndAdjustAppPrivileges();

    scanProcessesAndPopulateSelectionCombo();

    ui->statusBar->showMessage("Server stopped; Game offline; Game service not loaded");

    connect(ui->procRefreshButton, &QPushButton::clicked, this, [this]() {
        scanProcessesAndPopulateSelectionCombo();
    });

    connect(ui->loadButton, &QPushButton::clicked, this, &NecromancyLoaderWindow::onInjectButtonPressed);

    connect(_injector, &WinDllInjector::injectorExited, this, &NecromancyLoaderWindow::onInternalInjectorProcessFinished);

    connect(ui->sendingRateSlider, &QSlider::valueChanged, this, &NecromancyLoaderWindow::onSendingRateChanged);

    ui->sendingRateSlider->setValue(ui->sendingRateSlider->minimum());
}

NecromancyLoaderWindow::~NecromancyLoaderWindow() {
    delete ui;
}

void NecromancyLoaderWindow::onInjectButtonPressed()
{
    auto procInfo = ui->gameProcCombo->currentData().value<ProcessInfo*>();
    _injector->setTargetProcPid(procInfo->processId());

    if(_injector->hasTargetLoadedLibrary()) {
        return;
    }

    auto fullDllPath = locateReaderDll(_properties->value("general/pluginDll").toString());

    _injector->setTargetLibrary(fullDllPath.replace("/", "\\").toStdWString());

    if(!_pluginLoaded) {
        _injector->inject();
        _pluginLoaded = true;
        ui->loadButton->setText("Unload");
    } else {
        _injector->free();
        _pluginLoaded = false;
        ui->loadButton->setText("Load");
    }
}

void NecromancyLoaderWindow::onInternalInjectorProcessFinished(int exitCode, const QString& stdOut) const {
    ui->statusBar->showMessage(QString("Injector finished") + QString::number(exitCode) + "out: " + stdOut);
}

void NecromancyLoaderWindow::onSendingRateChanged(int value) {
    if(value == ui->sendingRateSlider->minimum()) {
        ui->leftSendingRateSpeedLabel->setText("Immediate");
        _immediateSendingRateWasReached = true;
        // todo: configure WebSocketServer delays
        return;
    }

    // todo: configure WebSocketServer

    if(_immediateSendingRateWasReached) { // if this flag is set and we get there it only means that new value is not maximum
        ui->leftSendingRateSpeedLabel->setText("Faster");
    }
}

void NecromancyLoaderWindow::loadProperties() {
    _properties = new QSettings("props.ini", QSettings::IniFormat);
}

void NecromancyLoaderWindow::scanProcessesAndPopulateSelectionCombo() {
    ui->gameProcCombo->clear();

    swapScannedProcesses(ProcessUtils::listActiveProcesses());
    int audiosurfProcId = 0;
    int counter = 0;
    for(auto proc : _scannedProcesses) {
        // todo: default icons for unregistered application types
        auto iconPixmap = QPixmap::fromImage(proc->icon());
        QIcon icon(iconPixmap);

        ui->gameProcCombo->addItem(icon, proc->processName(), QVariant::fromValue(proc));
        if(proc->processName().toLower() == "questviewer.exe") {
            audiosurfProcId = counter;
        }

        counter++;
    }

    ui->gameProcCombo->setCurrentIndex(audiosurfProcId);
}

void NecromancyLoaderWindow::swapScannedProcesses(const QList<ProcessInfo*> &newScannedProcesses) {
    if(!_scannedProcesses.isEmpty()) {
        for(auto procInfo : _scannedProcesses) {
            delete procInfo;
        }
    }

    _scannedProcesses.clear();

    for(auto procInfo : newScannedProcesses) {
        _scannedProcesses.append(procInfo);
    }
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

void NecromancyLoaderWindow::startWebSocketServer() const {
}

QString NecromancyLoaderWindow::locateReaderDll(const QString& targetFile) {
    QDir currentDir(QCoreApplication::applicationDirPath());

    QString filePath = currentDir.filePath(targetFile);
    if(QFileInfo::exists(filePath)) {
        return QFileInfo(filePath).absoluteFilePath();
    }

    return {};
}
