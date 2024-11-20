#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_necromancyloaderwindow.h"
#include "processinfo.h"

QT_BEGIN_NAMESPACE
namespace Ui { class NecromancyLoaderWindowClass; };
QT_END_NAMESPACE

class NecromancyLoaderWindow : public QMainWindow
{
    Q_OBJECT

public:
    NecromancyLoaderWindow(QWidget *parent = nullptr);
    ~NecromancyLoaderWindow();

private:
    void scanProcessesAndPopulateSelectionCombo();
    void checkAndAdjustAppPrivileges();

    QList<ProcessInfo*> listActiveProcesses();

    Ui::NecromancyLoaderWindowClass *ui;
};
