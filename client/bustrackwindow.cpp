#include "bustrackwindow.h"
#include "ui_bustrackwindow.h"

BusTrackWindow::BusTrackWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::BusTrackWindow)
{
    ui->setupUi(this);
    setMap();
    currentZoom = 1.0;
    slideValue = 0;
    connect(ui->zoomSlider, SIGNAL(valueChanged(int)), this, SLOT(zoomSlide(int)));
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
    static const float zoomFactor = 1.15;
    static const float minZoom = 1.0;

    if(event->delta() > 0)
    {
        // zoom in
        scaleMap(zoomFactor);
        currentZoom *= zoomFactor;
    }
    else if(currentZoom > minZoom)
    {
        // zoom out
        scaleMap(1.0/zoomFactor);
        currentZoom /= zoomFactor;
    }

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
    qDebug() << currentZoom;
    ui->mapView->scale(zoom, zoom);
}


void BusTrackWindow::zoomSlide(int newZoom)
{
    static const float zoomFactor = 1.15;
    static const float minZoom = 1.0;
    if (newZoom > slideValue){
        scaleMap(zoomFactor);
        currentZoom *= zoomFactor;
    } else if (newZoom < slideValue) {
        scaleMap(1.0/zoomFactor);
        currentZoom /= zoomFactor;
    }
    slideValue = newZoom;
}
