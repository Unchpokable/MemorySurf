#include "asmemoryreader.h"

ASMemoryReader::ASMemoryReader(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::ASMemoryReaderClass())
{
    ui->setupUi(this);
}

ASMemoryReader::~ASMemoryReader()
{
    delete ui;
}
