#include "bustrackwindow.h"
#include "ui_bustrackwindow.h"

BusTrackWindow::BusTrackWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::BusTrackWindow)
{
    ui->setupUi(this);
    setMap();
    currentZoom = 1.0;
    slideValue = 1.0;
    connect(ui->zoomSlider, SIGNAL(valueChanged(int)), this, SLOT(zoomSlide(int)));

    drawStop(100,100,30);
    drawStop(800,900,10);
    drawStop(1000,300,20);
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
    static const float zoomFactor = 1.2;
    static const float minZoom = 1.0;
    static const float maxZoom = 6.0;

    if(event->delta() > 0 && currentZoom <= maxZoom)
    {
        // zoom in
        scaleMap(zoomFactor);
        ui->zoomSlider->setValue(ui->zoomSlider->value()+1);
        currentZoom *= zoomFactor;
    }
    else if(currentZoom > minZoom)
    {
        // zoom out
        scaleMap(1.0/zoomFactor);
        ui->zoomSlider->setValue(ui->zoomSlider->value()-1);
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
    static const float zoomFactor = 1.2;
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

void BusTrackWindow::drawStop(int offsetx, int offsety, int numPeople)
{
    QPixmap stopPixmap;
    if (numPeople >= 20) {
	stopPixmap = QPixmap(":/resources/stopA.png");
    } else if (numPeople >= 10) {
	stopPixmap = QPixmap(":/resources/stopC.png");
    } else
        stopPixmap = QPixmap(":/resources/stopB.png");
    stopPixmap = stopPixmap.scaledToHeight(30, Qt::SmoothTransformation);
    QGraphicsPixmapItem* stopGraphics = new QGraphicsPixmapItem(stopPixmap);
    stopGraphics->setOffset(offsetx, offsety);
    mapScene.addItem(stopGraphics);
}
