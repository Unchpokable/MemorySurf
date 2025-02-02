#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_necromancyloaderwindow.h"

#include "processinfo.h"

#include <set>

class Injector;
class SharedMemoryReader;
class WebSocketBroadcastServer;
class DragHandler;

QT_BEGIN_NAMESPACE
namespace Ui { class NecromancyLoaderWindowClass; };
QT_END_NAMESPACE

class NecromancyLoaderWindow : public QMainWindow {
    Q_OBJECT

public:
    Q_DISABLE_COPY_MOVE(NecromancyLoaderWindow)

    explicit NecromancyLoaderWindow(QWidget *parent = nullptr);

    virtual ~NecromancyLoaderWindow() override;

private slots:
    void onInjectButtonPressed();
    void onInternalInjectorProcessFinished(int exitCode, const QString& stdOut) const;
    void onSendingRateChanged(int value);

    // ui validation
    void onPortEntered() const;
    void onPortValidationTimer() const;

    // initialization
    void onIpcChannelConnectionTimedOut() const;

private:
    // generic statics
    static QString locateReaderDll(const QString& targetFile);

    // ui validation implementation
    static bool isPortAllowed(quint16 port);
    inline static constexpr quint16 _defaultPort = 20080;
    inline static constexpr quint16 _forbiddenSystemPorts = 1024;
    static std::set<quint16> _forbiddenExternalPorts;

    void freezeServerStartUiComponents() const;
    void unfreezeServerStartUiComponent() const;

    // generic functions
    void loadProperties();
    void scanProcessesAndPopulateSelectionCombo();
    void swapScannedProcesses(const QList<ProcessInfo*> &newScannedProcesses);
    void checkAndAdjustAppPrivileges();
    void startWebSocketServer();

    // ui validation fields
    QTimer* _portEntryValidationTimer;

    // ui dragging
    DragHandler* _dragHandler;

    // generic data
    Injector* _injector;
    QList<ProcessInfo*> _scannedProcesses;
    Ui::NecromancyLoaderWindowClass *ui;
    QSettings* _properties;

    // IPC handlers
    SharedMemoryReader* _ipcChannel;

    // websocket
    WebSocketBroadcastServer* _server;

    // toggle states
    bool _pluginLoaded;
    bool _serverRunning;

    // sending rate configurations
    bool _immediateSendingRateWasReached { true }; // immediate mode is default
};
