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

std::unordered_map<quint16, QString> NecromancyLoaderWindow::_forbiddenExternalPorts = {
    {1080,  "SOCKS Proxy"},
    {1433,  "Microsoft SQL Server"},
    {1521,  "Oracle DB"},
    {1723,  "PPTP VPN"},
    {1883,  "MQTT"},
    {2049,  "NFS"},
    {2181,  "Zookeeper"},
    {2379,  "etcd"},
    {2380,  "etcd (peer communication)"},
    {2480,  "OrientDB"},
    {3306,  "MySQL"},
    {3389,  "RDP"},
    {3690,  "Subversion"},
    {4369,  "RabbitMQ/Erlang"},
    {5000,  "Flask Development Server / UPnP"},
    {5432,  "PostgreSQL"},
    {5672,  "RabbitMQ"},
    {6379,  "Redis"},
    {8000,  "HTTP (Development servers)"},
    {8080,  "HTTP (Alternative)"},
    {8443,  "HTTPS (Alternative)"},
    {9000,  "SonarQube"},
    {9092,  "Kafka"},
    {9200,  "Elasticsearch"},
    {9300,  "Elasticsearch (Transport)"},
    {11211, "Memcached"},
    {27017, "MongoDB"},
    {50051, "gRPC"}
};

NecromancyLoaderWindow::NecromancyLoaderWindow(QWidget *parent)
        : QMainWindow(parent), _injector(new Injector(this)), ui(new Ui::NecromancyLoaderWindowClass()) {
    setAttribute(Qt::WA_TranslucentBackground);
    QGraphicsDropShadowEffect* shadow = new QGraphicsDropShadowEffect;
    shadow->setBlurRadius(10);
    shadow->setOffset(0);
    shadow->setColor(QColor(0, 0, 0, 160));
    this->setGraphicsEffect(shadow);

    ui->setupUi(this);

    QPixmap icon(":/icons/appicon");
    auto scaledIcon = icon.scaled(32, 32, Qt::KeepAspectRatio, Qt::SmoothTransformation);

    ui->closeButton->resize(32, 32);

    connect(ui->closeButton, &QPushButton::clicked, this, [this]() {
        // todo: close all system resources before closing
        close();
    });

    ui->rightsIndicator->setRenderHint(QPainter::Antialiasing);
    ui->gameIndicator->setRenderHint(QPainter::Antialiasing);
    ui->pluginIndicator->setRenderHint(QPainter::Antialiasing);
    ui->serverIndicator->setRenderHint(QPainter::Antialiasing);

    ui->webSocketPortEntry->setText("20080");

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

    connect(ui->procRefreshButton, &QPushButton::clicked, this, [this]() {
        scanProcessesAndPopulateSelectionCombo();
    });

    connect(ui->loadButton, &QPushButton::clicked, this, &NecromancyLoaderWindow::onInjectButtonPressed);
    connect(ui->startServerButton, &QPushButton::clicked, this, &NecromancyLoaderWindow::onServerStartButtonPressed);

    connect(_injector, &Injector::injectorExited, this, &NecromancyLoaderWindow::onInternalInjectorProcessFinished);

    connect(ui->sendingRateSlider, &QSlider::valueChanged, this, &NecromancyLoaderWindow::onSendingRateChanged);

    connect(ui->webSocketPortEntry, &QLineEdit::textChanged, this, &NecromancyLoaderWindow::onPortEntered);
    connect(_portEntryValidationTimer, &QTimer::timeout, this, &NecromancyLoaderWindow::onPortValidationTimer);
    ui->sendingRateSlider->setValue(ui->sendingRateSlider->minimum());

    _dragHandler = new DragHandler(this);

    ui->centralWidget->installEventFilter(_dragHandler);

    _server = new WebSocketBroadcastServer(this, _defaultPort);

    _ipcChannel = new SharedMemoryReader(this);
    _ipcChannel->setBufferReadInterval(30);

    connect(_ipcChannel, &SharedMemoryReader::initializationTimedOut, this, &NecromancyLoaderWindow::onIpcChannelConnectionTimedOut);
    connect(_ipcChannel, &SharedMemoryReader::initialized, this, &NecromancyLoaderWindow::onIpcChannelInitialized);
    connect(_ipcChannel, &SharedMemoryReader::messageAcquired, _server, &WebSocketBroadcastServer::messageAcquired);

    _timerWaitUntilGameStarts = new QTimer(this);
    _timerWaitUntilGameStarts->setSingleShot(true);
    _timerWaitUntilGameStarts->setInterval(100);
    connect(_timerWaitUntilGameStarts, &QTimer::timeout, this, &NecromancyLoaderWindow::onGameWaitTimer);

    setupGraphicsIndicators();
}

NecromancyLoaderWindow::~NecromancyLoaderWindow() {
    delete ui;
}

bool NecromancyLoaderWindow::injectPlugin() {
    if(_injector->hasTargetLoadedLibrary()) {
        return false;
    }

    auto fullDllPath = locateReaderDll(_properties->value("engine/pluginDll").toString());

    _injector->setTargetLibrary(fullDllPath.replace("/", "\\").toStdWString());

    if(!_pluginLoaded) {
        _injector->inject();
        _pluginLoaded = true;
        ui->loadButton->setText("Unload");
    } else {
        _injector->free();
        _pluginLoaded = false;
        makeEllipseIndicator(IndicatorState::Bad, ui->pluginIndicator);
        ui->loadButton->setText("Load");
    }

    return true;
}

void NecromancyLoaderWindow::onInjectButtonPressed() {
    if(!_gameExists) {
        QProcess::startDetached("cmd.exe", { "/c", "start", "steam://rungameid/12900" });
        _timerWaitUntilGameStarts->start();
        return;
    }

    auto procInfo = ui->gameProcCombo->currentData().value<ProcessInfo*>();
    _injector->setTargetProcPid(procInfo->processId());

    injectPlugin();
}

void NecromancyLoaderWindow::onInternalInjectorProcessFinished(int exitCode, const QString& stdOut) const {
    if(exitCode != 0) {
        QMessageBox::critical(nullptr, "Injector error", QString("Injector process returned code %1. " 
            "This can happens when you launch a loader without admin rights. Please, re-run loader as admin and try again. "
            "For debug purposes, here is a Injector' stderr and stdout data:\n%2").arg(exitCode).arg(stdOut));
    }

    makeEllipseIndicator(IndicatorState::Fine, ui->gameIndicator);
    ui->loadButton->setText("Unload");

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
    _ipcChannel->startInit();
}

void NecromancyLoaderWindow::onIpcChannelInitialized() const {
    makeEllipseIndicator(IndicatorState::Fine, ui->pluginIndicator);
}

void NecromancyLoaderWindow::onServerStartButtonPressed() {
    if(_serverRunning) {
        if(_server->stop()) {
            ui->startServerButton->setText("Start server");
            _serverRunning = false;
            makeEllipseIndicator(IndicatorState::Bad, ui->serverIndicator);
        } else {
            QMessageBox::warning(this, "Ooops!", "Unable to start or stop server. Please, restart the application");
        }
    } else {
        if(_server->start()) {
            ui->startServerButton->setText("Stop server");
            _serverRunning = true;
            makeEllipseIndicator(IndicatorState::Fine, ui->serverIndicator);
        } else {
            QMessageBox::warning(this, "Ooops!", "Unable to start or stop server. Please, restart the application");
        }
    }
}

void NecromancyLoaderWindow::onGameWaitTimer() {
    auto gamePid = ProcessUtils::findProcessNamed("QuestViewer.exe");
    if(gamePid == 0) {
        _timerWaitUntilGameStarts->start();
        return;
    }

    _gameExists = true;

    scanProcessesAndPopulateSelectionCombo();

    QTimer::singleShot(5000, [this, gamePid] {
        _injector->setTargetProcPid(gamePid);
        injectPlugin();
    });

}

QString NecromancyLoaderWindow::locateReaderDll(const QString& targetFile) {
    QDir currentDir(QCoreApplication::applicationDirPath());

    QString filePath = currentDir.filePath(targetFile);
    if(QFileInfo::exists(filePath)) {
        return QFileInfo(filePath).absoluteFilePath();
    }

    return {};
}

QGraphicsEllipseItem* NecromancyLoaderWindow::makeEllipseIndicator(IndicatorState desiredState, QGraphicsView* graphicsView) {
    if(!graphicsView) {
        qDebug() << "No graphics view!";
        return nullptr;
    }

    if(!graphicsView->scene()) {
        QGraphicsScene* newScene = new QGraphicsScene(graphicsView);
        graphicsView->setScene(newScene);
    }

    QGraphicsEllipseItem* ellipse = new QGraphicsEllipseItem(0, 0, 10, 10);

    QColor color;
    switch(desiredState) {
    case IndicatorState::Bad: 
        color = QColor(255, 107, 107); 
        break;
    case IndicatorState::Well:
        color = QColor(255, 217, 102);
        break;
    case IndicatorState::Fine:
        color = QColor(119, 221, 119);
        break;
    case IndicatorState::Good:
        color = QColor(102, 153, 255);
        break;
    }

    ellipse->setBrush(color);
    ellipse->setPen(Qt::NoPen); 

    QGraphicsDropShadowEffect* glow = new QGraphicsDropShadowEffect;
    glow->setBlurRadius(15);
    glow->setColor(color.lighter(150));
    glow->setOffset(0, 0);
    ellipse->setGraphicsEffect(glow);

    QPoint viewCenter = graphicsView->viewport()->rect().center();
    QPointF scenePos = graphicsView->mapToScene(viewCenter);
    ellipse->setPos(scenePos);

    graphicsView->scene()->clear();
    graphicsView->scene()->addItem(ellipse);

    return ellipse;
}

bool NecromancyLoaderWindow::isPortAllowed(quint16 port) {
    if(port < _forbiddenSystemPorts) {
        return false;
    }

    return _forbiddenExternalPorts.find(port) == _forbiddenExternalPorts.end();
}

void NecromancyLoaderWindow::freezeServerStartUiComponents() const {
    ui->startServerButton->setEnabled(false);
    QString reason;
    quint16 port = ui->webSocketPortEntry->text().toInt();
    if(port <= _forbiddenSystemPorts) {
        reason = QString("Port %1 is a system port, please provide user port (>= 1024)").arg(port);
    } else {
        reason = QString("Port %1 can be used by %2, please provide unused port").arg(port)
            .arg(_forbiddenExternalPorts[port]);
    }
    ui->startServerButton->setToolTip(reason);
    ui->webSocketPortEntry->setProperty("validationState", "invalid");
}

void NecromancyLoaderWindow::unfreezeServerStartUiComponent() const {
    ui->startServerButton->setEnabled(true);
    ui->startServerButton->setToolTip("");
    ui->webSocketPortEntry->setProperty("validationState", "invalid");
}

void NecromancyLoaderWindow::loadProperties() {
    QString iniPath = QDir(QCoreApplication::applicationDirPath()).filePath("props.ini");
    _properties = new QSettings(iniPath, QSettings::IniFormat);
    _properties->sync();
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
            _gameExists = true;
        }

        counter++;
    }

    ui->gameProcCombo->setCurrentIndex(audiosurfProcId);

    _gameExists = audiosurfProcId != 0;

    if(!_gameExists) {
        ui->loadButton->setText("Launch via Steam");
        hideLayout(ui->gameSelectionLayout);
    } else {
        ui->loadButton->setText("Load");
        showLayout(ui->gameSelectionLayout);
    }
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

void NecromancyLoaderWindow::checkAndAdjustAppPrivileges() const {
    WinHandle token;
    if(!OpenProcessToken(GetCurrentProcess(), TOKEN_QUERY, &token)) {
        makeEllipseIndicator(IndicatorState::Bad, ui->rightsIndicator);
        ui->appRightsLabel->setText("App privilege is bad!!");
        return;
    }

    TOKEN_ELEVATION elevation;
    WinDword size;
    if(!GetTokenInformation(token, TokenElevation, &elevation, sizeof(elevation), &size)) {
        CloseHandle(token);
        makeEllipseIndicator(IndicatorState::Bad, ui->rightsIndicator);
        ui->appRightsLabel->setText("App privilege is bad!!");
        return;
    }

    CloseHandle(token);
    if(elevation.TokenIsElevated == 0) {
        makeEllipseIndicator(IndicatorState::Well , ui->rightsIndicator);
        ui->rightsIndicator->setToolTip("Loader running as user. Mostly enough, but can lead to some issues sometimes");
        ui->appRightsLabel->setText("User");
        return;
    }

    makeEllipseIndicator(IndicatorState::Fine, ui->rightsIndicator);
    ui->rightsIndicator->setToolTip("Loader running as Admin. Good for 99% cases. Enjoy Necromancy :>");
    ui->appRightsLabel->setText("Admin");

    auto isAdjusted = RaisePrivilegesToDebug();

    if(isAdjusted) {
        makeEllipseIndicator(IndicatorState::Good, ui->rightsIndicator);
        ui->rightsIndicator->setToolTip("Loader running with all privileges that it needed. Enjoy Necromancy :>");
        ui->appRightsLabel->setText("Admin");
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

void NecromancyLoaderWindow::hideLayout(const QLayout* layout) {
    auto itemCount = layout->count();

    for(int i { 0 }; i < itemCount; i++) {
        auto item = layout->itemAt(i);
        if(auto innerLayout = item->layout()) {
            hideLayout(innerLayout);
        }
        if(item->widget()) {
            item->widget()->hide();
        }
    }
}

void NecromancyLoaderWindow::showLayout(const QLayout* layout) {
    auto itemCount = layout->count();

    for(int i { 0 }; i < itemCount; i++) {
        auto item = layout->itemAt(i);
        if(auto innerLayout = item->layout()) {
            showLayout(innerLayout);
        }
        if(item->widget()) {
            item->widget()->show();
        }
    }
}

void NecromancyLoaderWindow::setupGraphicsIndicators() {
    // todo: real implementation

    makeEllipseIndicator(IndicatorState::Bad, ui->gameIndicator);
    makeEllipseIndicator(IndicatorState::Bad, ui->pluginIndicator);
    makeEllipseIndicator(IndicatorState::Bad, ui->serverIndicator);
}
