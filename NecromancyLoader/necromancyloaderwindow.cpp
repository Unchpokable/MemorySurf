#include "pch.h"
#include "necromancyloaderwindow.h"

NecromancyLoaderWindow::NecromancyLoaderWindow(QWidget *parent)
        : QMainWindow(parent)
        , ui(new Ui::NecromancyLoaderWindowClass()) {
    ui->setupUi(this);
}

NecromancyLoaderWindow::~NecromancyLoaderWindow() {
    delete ui;
}
