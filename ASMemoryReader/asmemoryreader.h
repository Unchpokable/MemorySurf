#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_asmemoryreader.h"

QT_BEGIN_NAMESPACE
namespace Ui { class ASMemoryReaderClass; };
QT_END_NAMESPACE

class ASMemoryReader : public QMainWindow
{
    Q_OBJECT

public:
    ASMemoryReader(QWidget *parent = nullptr);
    ~ASMemoryReader();

private:
    Ui::ASMemoryReaderClass *ui;
};
