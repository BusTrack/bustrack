#include "bustrackwindow.h"
#include "ui_bustrackwindow.h"

BusTrackWindow::BusTrackWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::BusTrackWindow)
{
    ui->setupUi(this);
    mapPixmap = QPixmap(":/resources/map.png");
    mapScene.addPixmap(mapPixmap);
    ui->mapView->setScene(&mapScene);
}

BusTrackWindow::~BusTrackWindow()
{
    //testing commit //terence
    delete ui;
}

