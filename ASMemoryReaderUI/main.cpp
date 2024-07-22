#include "ASMemoryReaderUI.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ASMemoryReaderUI w;
    w.show();
    return a.exec();
}
