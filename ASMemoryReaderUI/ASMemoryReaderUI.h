#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_ASMemoryReaderUI.h"

QT_BEGIN_NAMESPACE

namespace Ui
{
    class ASMemoryReaderUIClass;
};

QT_END_NAMESPACE

class ASMemoryReaderUI : public QMainWindow
{
    Q_OBJECT

public:
    ASMemoryReaderUI(QWidget* parent = nullptr);
    ~ASMemoryReaderUI() override;

private:
    Ui::ASMemoryReaderUIClass* ui;
};
