#include "pch.h"
#include "necromancyloaderwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    NecromancyLoaderWindow w;
    w.show();
    return a.exec();
}
