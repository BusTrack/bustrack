#include "bustrackwindow.h"
#include "ui_bustrackwindow.h"

const float ZOOM_FACTOR = 1.2;
const float MIN_ZOOM = 1.0;
const float MAX_ZOOM = 6.0;

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
    if(event->delta() > 0 && currentZoom <= MAX_ZOOM)
    {
        // zoom in
        scaleMap(ZOOM_FACTOR);
        ui->zoomSlider->setValue(ui->zoomSlider->value()+1);
        currentZoom *= ZOOM_FACTOR;
    }
    else if(currentZoom > MIN_ZOOM)
    {
        // zoom out
        scaleMap(1.0/ZOOM_FACTOR);
        ui->zoomSlider->setValue(ui->zoomSlider->value()-1);
        currentZoom /= ZOOM_FACTOR;
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
    if (newZoom > slideValue){
        scaleMap(ZOOM_FACTOR);
        currentZoom *= ZOOM_FACTOR;
    } else if (newZoom < slideValue) {
        scaleMap(1.0/ZOOM_FACTOR);
        currentZoom /= ZOOM_FACTOR;
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
