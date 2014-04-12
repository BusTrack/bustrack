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

void BusTrackWindow::mousePressEvent( QMouseEvent* event )
{
    if (QGraphicsItem *item = ui->mapView->itemAt(event->pos())) {
        qDebug() << "You clicked on item" << item;
        QList<QGraphicsItem *> childList = item->childItems();
        qDebug() << childList;
        for (int i = 0; i < childList.size(); i++){
            if (childList[i]->isVisible()){
                childList[i]->hide();
            } else {
                childList[i]->show();
            }
        }
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
    QPixmap busstopPixmap;
    if (numPeople >= 20) {
	busstopPixmap = QPixmap(":/resources/stopA.png");
    } else if (numPeople >= 10) {
	busstopPixmap = QPixmap(":/resources/stopC.png");
    } else
        busstopPixmap = QPixmap(":/resources/stopB.png");
    busstopPixmap = busstopPixmap.scaledToHeight(30, Qt::SmoothTransformation);
    QGraphicsPixmapItem* busstopGraphics = new QGraphicsPixmapItem(busstopPixmap);

    QGraphicsRectItem* busstopinfoGraphics = new QGraphicsRectItem(offsetx+40, offsety-25, 150, 180);
    busstopinfoGraphics->setBrush(Qt::cyan);
    QGraphicsSimpleTextItem* busstopName = new QGraphicsSimpleTextItem("COM2");
    busstopName->setPos(offsetx+50,offsety-15);
    busstopName->setParentItem(busstopinfoGraphics);

    busstopinfoGraphics->hide();
    busstopinfoGraphics->setParentItem(busstopGraphics);
    busstopGraphics->setOffset(offsetx, offsety);
    mapScene.addItem(busstopGraphics);
}
