#include "pch.h"
#include "necromancyloaderwindow.h"

NecromancyLoaderWindow::NecromancyLoaderWindow(QWidget *parent)
        : QMainWindow(parent)
        , ui(new Ui::NecromancyLoaderWindowClass()) {
    ui->setupUi(this);
    setWindowTitle("Necromancy Loader");
}

NecromancyLoaderWindow::~NecromancyLoaderWindow() {
    delete ui;
}
