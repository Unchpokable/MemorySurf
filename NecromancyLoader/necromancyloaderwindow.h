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

    enum class IndicatorState {
        Bad, // red indicator
        Well,// yellow indicator
        Fine,// green indicator
        Good // blue indicator
    };

public:
    Q_DISABLE_COPY_MOVE(NecromancyLoaderWindow)

    explicit NecromancyLoaderWindow(QWidget* parent = nullptr);

    virtual ~NecromancyLoaderWindow() override;
    bool injectPlugin();

private slots:
    void onInjectButtonPressed();
    void onInternalInjectorProcessFinished(int exitCode, const QString& stdOut) const;
    void onSendingRateChanged(int value);

    // ui validation
    void onPortEntered() const;
    void onPortValidationTimer() const;

    // initialization
    void onIpcChannelConnectionTimedOut() const;
    void onIpcChannelInitialized() const;

    // web socket server
    void onServerStartButtonPressed();

    // game loading and tracking
    void onGameWaitTimer();
    void onGameTrackTimer();


    // controlling trackers
    void onLaunchTrackerButtonClicked() const;

private:
    // generic statics
    static QString locateReaderDll(const QString& targetFile);

    // indicators (static)
    static QGraphicsEllipseItem* makeEllipseIndicator(IndicatorState desiredState, QGraphicsView* graphicsView);

    // ui validation implementation
    static bool isPortAllowed(quint16 port);
    inline static constexpr quint16 _defaultPort = 20080;
    inline static constexpr quint16 _forbiddenSystemPorts = 1024;
    static std::unordered_map<quint16, QString> _forbiddenExternalPorts;

    void freezeServerStartUiComponents() const;
    void unfreezeServerStartUiComponent() const;

    // generic functions
    void loadProperties();
    void scanProcessesAndPopulateSelectionCombo();
    void swapScannedProcesses(const QList<ProcessInfo*> &newScannedProcesses);
    void checkAndAdjustAppPrivileges() const;
    void startWebSocketServer();

    static void hideLayout(const QLayout* layout);
    static void showLayout(const QLayout* layout);

    // indicators
    void setupGraphicsIndicators();

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
    bool _gameRunning;
    bool _pluginLoaded;
    bool _serverRunning;

    // game control and auto launch
    bool _gameExists;
    QTimer* _timerWaitUntilGameStarts;
    QTimer* _gameTrackingTimer;

    // sending rate configurations
    bool _immediateSendingRateWasReached { true }; // immediate mode is default

    // trackers plugins
    QProcess* _trackerHandle;
};
