#include "ASMemoryReaderUI.h"

ASMemoryReaderUI::ASMemoryReaderUI(QWidget* parent)
    : QMainWindow(parent)
      , ui(new Ui::ASMemoryReaderUIClass())
{
    ui->setupUi(this);
}

ASMemoryReaderUI::~ASMemoryReaderUI()
{
    delete ui;
}
