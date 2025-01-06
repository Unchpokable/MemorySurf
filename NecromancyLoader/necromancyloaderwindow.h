#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_necromancyloaderwindow.h"
#include "processinfo.h"

class WinDllInjector;
QT_BEGIN_NAMESPACE
namespace Ui { class NecromancyLoaderWindowClass; };
QT_END_NAMESPACE

class NecromancyLoaderWindow : public QMainWindow
{
    Q_OBJECT

public:
    NecromancyLoaderWindow(QWidget *parent = nullptr);
    virtual ~NecromancyLoaderWindow() override;

private slots:
    void onInjectButtonPressed() const;
    void onUnloadButtonPressed() const;
    void onInternalInjectorProcessFinished(int exitCode, const QString& stdOut) const;

private:
    void loadProperties();
    void scanProcessesAndPopulateSelectionCombo();
    void swapScannedProcesses(const QList<ProcessInfo*> &newScannedProcesses);
    void checkAndAdjustAppPrivileges();
    void startWebSocketServer() const;
    static QString locateReaderDll(const QString& targetFile);

    WinDllInjector* _injector;
    QList<ProcessInfo*> _scannedProcesses;
    Ui::NecromancyLoaderWindowClass *ui;
    QSettings* _properties;
};
