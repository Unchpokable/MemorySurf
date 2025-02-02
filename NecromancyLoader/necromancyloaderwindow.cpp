#include "pch.h"

#include "necromancyloaderwindow.h"
#include "draghandler.h"

#include "injector.h"
#include "privileges.h"
#include "processinfo.h"
#include "processutils.h"
#include "sharedmemoryreader.h"
#include "websocketbroadcastserver.h"
#include "windefprettify.h"

std::set<quint16> NecromancyLoaderWindow::_forbiddenExternalPorts = {
    1080,  // SOCKS Proxy
    1433,  // Microsoft SQL Server
    1521,  // Oracle DB
    1723,  // PPTP VPN
    1883,  // MQTT
    2049,  // NFS
    2181,  // Zookeeper
    2379,  // etcd
    2380,  // etcd (peer communication)
    2480,  // OrientDB
    3306,  // MySQL
    3389,  // RDP
    3690,  // Subversion
    4369,  // RabbitMQ/Erlang
    5000,  // Flask Development Server / UPnP
    5432,  // PostgreSQL
    5672,  // RabbitMQ
    6379,  // Redis
    8000,  // HTTP (Development servers)
    8080,  // HTTP (Alternative)
    8443,  // HTTPS (Alternative)
    9000,  // SonarQube
    9092,  // Kafka
    9200,  // Elasticsearch
    9300,  // Elasticsearch (Transport)
    11211, // Memcached
    27017, // MongoDB
    50051, // gRPC
};

NecromancyLoaderWindow::NecromancyLoaderWindow(QWidget *parent)
        : QMainWindow(parent), _injector(new Injector(this)), ui(new Ui::NecromancyLoaderWindowClass()) {
    ui->setupUi(this);

    QPixmap icon(":/icons/appicon");
    auto scaledIcon = icon.scaled(32, 32, Qt::KeepAspectRatio, Qt::SmoothTransformation);

    ui->closeButton->resize(32, 32);

    connect(ui->closeButton, &QPushButton::clicked, this, [this]() {
        // todo: close all system resources before closing
        close();
    });

    ui->iconLabel->setPixmap(scaledIcon);
    ui->iconLabel->setScaledContents(true);
    ui->iconLabel->resize(32, 32);

    qRegisterMetaType<ProcessInfo>();
    qRegisterMetaType<ProcessInfo*>();

    _portEntryValidationTimer = new QTimer(this);
    _portEntryValidationTimer->setInterval(100);
    _portEntryValidationTimer->setSingleShot(true);

    loadProperties();

    setWindowTitle("Necromancy Loader");
    checkAndAdjustAppPrivileges();

    scanProcessesAndPopulateSelectionCombo();

    ui->statusBar->showMessage("Server stopped; Game offline; Game service not loaded");

    connect(ui->procRefreshButton, &QPushButton::clicked, this, [this]() {
        scanProcessesAndPopulateSelectionCombo();
    });

    connect(ui->loadButton, &QPushButton::clicked, this, &NecromancyLoaderWindow::onInjectButtonPressed);

    connect(_injector, &Injector::injectorExited, this, &NecromancyLoaderWindow::onInternalInjectorProcessFinished);

    connect(ui->sendingRateSlider, &QSlider::valueChanged, this, &NecromancyLoaderWindow::onSendingRateChanged);

    connect(ui->webSocketPortEntry, &QLineEdit::textChanged, this, &NecromancyLoaderWindow::onPortEntered);
    connect(_portEntryValidationTimer, &QTimer::timeout, this, &NecromancyLoaderWindow::onPortValidationTimer);
    ui->sendingRateSlider->setValue(ui->sendingRateSlider->minimum());

    _dragHandler = new DragHandler(this);

    ui->centralWidget->installEventFilter(_dragHandler);

    _server = new WebSocketBroadcastServer(this, _defaultPort);
    ui->customHeader->setProperty("IsTopBar", true);

    _ipcChannel = new SharedMemoryReader(this);
    _ipcChannel->setBufferReadInterval(30);

    connect(_ipcChannel, &SharedMemoryReader::initializationTimedOut, this, &NecromancyLoaderWindow::onIpcChannelConnectionTimedOut);
    connect(_ipcChannel, &SharedMemoryReader::messageAcquired, _server, &WebSocketBroadcastServer::messageAcquired);
}

NecromancyLoaderWindow::~NecromancyLoaderWindow() {
    delete ui;
}

void NecromancyLoaderWindow::onInjectButtonPressed() {
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

    _ipcChannel->startInit();
}

void NecromancyLoaderWindow::onSendingRateChanged(int value) {
    if(value == ui->sendingRateSlider->minimum()) {
        ui->leftSendingRateSpeedLabel->setText("Immediate");
        _immediateSendingRateWasReached = true;
        return;
    }

    if(_immediateSendingRateWasReached) { // if this flag is set and we get there it only means that new value is not maximum
        ui->leftSendingRateSpeedLabel->setText("Faster");
    }
}

void NecromancyLoaderWindow::onPortEntered() const {
    _portEntryValidationTimer->start();
}

void NecromancyLoaderWindow::onPortValidationTimer() const {
    auto port = ui->webSocketPortEntry->text().toInt();
    if(port > std::numeric_limits<quint16>::max()) {
        freezeServerStartUiComponents();
    }

    auto allowed = isPortAllowed(static_cast<quint16>(port));

    if(!allowed) {
        freezeServerStartUiComponents();
    } else {
        unfreezeServerStartUiComponent();
    }
}

void NecromancyLoaderWindow::onIpcChannelConnectionTimedOut() const {
    ui->statusBar->showMessage("IPC connection timed out. Retrying...");
    _ipcChannel->startInit();
}

void NecromancyLoaderWindow::freezeServerStartUiComponents() const {
    ui->startServerButton->setEnabled(false);
    ui->webSocketPortEntry->setProperty("validationState", "invalid");
}

void NecromancyLoaderWindow::unfreezeServerStartUiComponent() const {
    ui->startServerButton->setEnabled(true);
    ui->webSocketPortEntry->setProperty("validationState", "invalid");
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

void NecromancyLoaderWindow::startWebSocketServer() {
    quint16 port { _defaultPort };
    if(!ui->webSocketPortEntry->text().isEmpty()) {
        port = ui->webSocketPortEntry->text().toInt();
    }

    _server = new WebSocketBroadcastServer(this, port);
    _server->set_packetSkip(ui->sendingRateSlider->value());
    auto serverStarted = _server->start();

    if(!serverStarted) {
        delete _server;
        QMessageBox::warning(this, "Server boot error", 
            "Server can't start with current parameters. Mostly it may be caused by wrong port number. Please, change port to any unused value and try again");
    }
}

QString NecromancyLoaderWindow::locateReaderDll(const QString& targetFile) {
    QDir currentDir(QCoreApplication::applicationDirPath());

    QString filePath = currentDir.filePath(targetFile);
    if(QFileInfo::exists(filePath)) {
        return QFileInfo(filePath).absoluteFilePath();
    }

    return {};
}

bool NecromancyLoaderWindow::isPortAllowed(quint16 port) {
    if(port < _forbiddenSystemPorts) {
        return false;
    }

    return _forbiddenExternalPorts.find(port) == _forbiddenExternalPorts.end();
}
