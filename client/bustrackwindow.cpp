#include "bustrackwindow.h"
#include "ui_bustrackwindow.h"

BusTrackWindow::BusTrackWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::BusTrackWindow)
{
    ui->setupUi(this);
    setMap();
}

BusTrackWindow::~BusTrackWindow()
{
    //testing commit //terence
    delete ui;
}

// event handlers

// override default mouse behaviour to implement zoom
void BusTrackWindow::wheelEvent(QWheelEvent *event)
{

}

void BusTrackWindow::resizeEvent(QResizeEvent *event)
{

}


void BusTrackWindow::setMap()
{
    mapPixmap = QPixmap(":/resources/map.png");
    mapScene.addPixmap(mapPixmap);
    ui->mapView->setScene(&mapScene);
}

void BusTrackWindow::scaleMap(float zoom)
{
    if(zoom < 0)
    {
        return;
    }
    else
    {
         ui->mapView->scale(zoom, zoom);
    }
}

