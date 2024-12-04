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
    ~NecromancyLoaderWindow();

private slots:
    void onInjectButtonPressed();
    void onUnloadButtonPressed();
    void onInternalInjectorProcessFinished(int exitCode, const QString& stdOut) const;

private:
    void scanProcessesAndPopulateSelectionCombo();
    void swapScannedProcesses(const QList<ProcessInfo*> &newScannedProcesses);
    void checkAndAdjustAppPrivileges();
    QString locateReaderDll(const QString& targetFile = "NecromancyEngineV2.dll"); // todo: replace hardcode to LoaderProperties

    WinDllInjector* _injector;
    QList<ProcessInfo*> _scannedProcesses;
    Ui::NecromancyLoaderWindowClass *ui;
};
