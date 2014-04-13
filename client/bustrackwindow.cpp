#include "bustrackwindow.h"
#include "ui_bustrackwindow.h"

const float ZOOM_FACTOR = 1.2;
const float MIN_ZOOM = 1.0;
const float MAX_ZOOM = 6.0;
const float MAX_NUM_PEOPLE_BUS = 50;

BusTrackWindow::BusTrackWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::BusTrackWindow)
{
    ui->setupUi(this);
    setMap();
    currentZoom = 1.0;
    slideValue = 1.0;
    connect(ui->zoomSlider, SIGNAL(valueChanged(int)), this, SLOT(zoomSlide(int)));

	setMouseTracking(true);

    drawStop(100,100,0);
    drawStop(800,900,1);
    drawStop(1000,300,15);
    
    drawBus("A1", 500, 400, 60);
    drawBus("B", 300, 400, 59);
    drawBus("A2", 200, 400, 30);
    drawBus("C", 100, 400, 2);
    drawBus("D2", 10, 400, 0);
}

BusTrackWindow::~BusTrackWindow()
{
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

void BusTrackWindow::mouseMoveEvent(QMouseEvent* event)
{
	if (QGraphicsItem *item = ui->mapView->itemAt(event->pos())) {
        qDebug() << "You hovered" << item;
        QList<QGraphicsItem *> childList = item->childItems();
        qDebug() << childList;
        for (int i = 0; i < childList.size(); i++){
            if (childList[i]->contains(event->pos())){
                qDebug() << "hover";
            } else {
                qDebug() << "nope";
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
    //Color transitioning from red to yellow to green
    QColor repaintColor;
    repaintColor.setRgb(255,0,36);
    if (numPeople < 30 && numPeople > 15) {
        repaintColor.setRgb(240.0/(numPeople-15), 240.0/(numPeople-15), 36);
    } else if (numPeople > 0) {
        repaintColor.setRgb(240 - (240.0/numPeople), (210.0/numPeople), 36);
    } else if (numPeople == 0) {
        repaintColor.setRgb(0,210,36);
    }

    //Locating pixels to be painted and replacing them with new color
    QImage background;
    QImage world(2359, 1738, QImage::Format_RGB32);
    QSize sizeImage;
    int r,g,b,alpha;
    QRgb color;
    int height, width;
    background.load(":/resources/stopA.png");
    world.fill(1);	 
    QPainter painter(&world);
    sizeImage = background.size();
    width = sizeImage.width();
    height = sizeImage.height();	 
    for(int y = 0; y < height; y++) {
        for(int x = 0; x < width; x++) {
            color = background.pixel(x,y);
            if (qRed(color) == 255 && qGreen(color) == 0 && qBlue(color) == 36) {
         	  background.setPixel(x,y,repaintColor.rgb());
   	    }
        }
    }
    painter.drawImage(0,0,background);

    //Conversion of image to pixmap and scale
    QPixmap busstopPixmap;
    busstopPixmap.convertFromImage(background);
    busstopPixmap = busstopPixmap.scaledToHeight(30, Qt::SmoothTransformation);

    //Generation of child graphics (toggle-able additional info)
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

void BusTrackWindow::drawBus(QString busService, float offsetx, float offsety, int numPeople)
{
	float percentageNumPeople = (numPeople * 1.0)/MAX_NUM_PEOPLE_BUS;
	//Color transitioning from red to yellow to green	
	QColor repaintBusColor;
	repaintBusColor.setRgb(255,0,36);	
	if (numPeople < 30 && numPeople > 15) {
		repaintBusColor.setRgb(240.0/(numPeople-15), 
			240.0/(numPeople-15), 36);
	} else if (numPeople > 0) {
		repaintBusColor.setRgb(240 - (240.0/numPeople), 
			(210.0/numPeople), 36);
	} else if (numPeople <= 0) {
		repaintBusColor.setRgb(0,210,36);
	}	

	//Locating pixels to be painted and replacing them with new color
	QImage world(2359, 1738, QImage::Format_RGB32);
	QSize sizeImage;
	int r,g,b,alpha;
	QRgb color;
	int height, width;
	QImage background = determineBusPNG(busService);
	world.fill(1);	 
	QPainter painter(&world);
	sizeImage = background.size();
	width = sizeImage.width();
	height = sizeImage.height();	 
	for(int y = 0; y < height; y++) {
		for(int x = 0; x < width; x++) {
		    color = background.pixel(x,y);
		    if (qRed(color) == 255 && qGreen(color) == 0 && qBlue(color) == 36) {
		 	  background.setPixel(x,y,repaintBusColor.rgb());
		    }
		}
	}
	painter.drawImage(0,0,background);

	//Conversion of image to pixmap and scale
	QPixmap busPixmap;
	busPixmap.convertFromImage(background);
	busPixmap = busPixmap.scaledToHeight(30, Qt::SmoothTransformation);
	QGraphicsPixmapItem* busGraphics = new QGraphicsPixmapItem(busPixmap);
	busGraphics->setOffset(offsetx, offsety);
   	mapScene.addItem(busGraphics);
}

QImage BusTrackWindow::determineBusPNG(QString busService){
	QImage background;
	
	if (busService == "A1"){
		background.load(":/resources/A1.png");	
	} else if (busService == "A2"){
		background.load(":/resources/A2.png");	
	} else if (busService == "B"){
		background.load(":/resources/B.png");	
	} else if (busService == "C"){
		background.load(":/resources/C.png");	
	} else if (busService == "D1"){
		background.load(":/resources/D1.png");	
	} else {
		background.load(":/resources/D2.png");	
	}

	return background;
}
