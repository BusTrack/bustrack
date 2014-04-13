#include "bustrackwindow.h"
#include "ui_bustrackwindow.h"

const float ZOOM_FACTOR = 1.2;
const float MIN_ZOOM = 1.0;
const float MAX_ZOOM = 6.0;
const float MAX_NUM_PEOPLE_BUS = 50;
const float BUS_LOW_OCCUPANCY = 0.3;
const float BUS_HIGH_OCCUPANCY = 0.7;

BusTrackWindow::BusTrackWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::BusTrackWindow)
{
    ui->setupUi(this);
    initializeWidgets();
    initializeConnections();
    initializeValues();

    drawStop(100,100,0);
    drawStop(800,900,10);
    drawStop(1000,300,16);
    
    drawStop(1000,300,20);
    drawBus("A1", 500, 300, 10);
    drawBus("C", 10, 400, 35);
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

void BusTrackWindow::toggleSearchResultsWidget(QString query)
{
    if(query.length() > 0)
    {
        ui->searchResultsWidget->setVisible(true);
    }
    else
    {
        ui->searchResultsWidget->setVisible(false);
    }
}

void BusTrackWindow::onBusInfoBtnClicked()
{

    if(!busInfoBtnClicked)
    {
        ui->busInfoBtn->setIcon(QIcon(":/resources/searchBusSel.png"));
        ui->stopInfoBtn->setIcon(QIcon(":/resources/searchStop.png"));
        ui->infoListWidget->move(79, 80);
        ui->infoListWidget->setVisible(true);
        busInfoBtnClicked = true;
    }
    else
    {
        ui->busInfoBtn->setIcon(QIcon(":/resources/searchBus.png"));
        busInfoBtnClicked = false;
        ui->infoListWidget->setVisible(false);
    }

}

void BusTrackWindow::onStopInfoBtnClicked()
{
    if(!stopInfoBtnClicked)
    {
        ui->stopInfoBtn->setIcon(QIcon(":/resources/searchStopSel.png"));
        ui->busInfoBtn->setIcon(QIcon(":/resources/searchBus.png"));
        ui->infoListWidget->move(79, 140);
        ui->infoListWidget->setVisible(true);
        stopInfoBtnClicked = true;
    }
    else
    {
        ui->stopInfoBtn->setIcon(QIcon(":/resources/searchStop.png"));
        ui->infoListWidget->setVisible(false);
        stopInfoBtnClicked = false;

    }
}

void BusTrackWindow::initializeWidgets()
{
    ui->searchResultsWidget->setVisible(false);
    ui->infoListWidget->setVisible(false);
    setMap();
}

void BusTrackWindow::initializeConnections()
{
    connect(ui->zoomSlider, SIGNAL(valueChanged(int)), this, SLOT(zoomSlide(int)));
    connect(ui->searchLineEdit, SIGNAL(textChanged(QString)), this, SLOT(toggleSearchResultsWidget(QString)));
    connect(ui->busInfoBtn, SIGNAL(clicked()), this, SLOT(onBusInfoBtnClicked()));
    connect(ui->stopInfoBtn, SIGNAL(clicked()), this, SLOT(onStopInfoBtnClicked()));
}

void BusTrackWindow::initializeValues()
{
    currentZoom = 1.0;
    slideValue = 1.0;
    busInfoBtnClicked = false;
    stopInfoBtnClicked = false;
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
	QPixmap busPixmap;
	
	if (busService == "A1"){
		if (percentageNumPeople < BUS_LOW_OCCUPANCY){
			busPixmap = QPixmap(":/resources/busA1Red.png");
		} else if (percentageNumPeople < BUS_HIGH_OCCUPANCY){
			busPixmap = QPixmap(":/resources/busA1Red.png");
		} else{
			busPixmap = QPixmap(":/resources/busA1Red.png");
		}
	} else if (busService == "A2"){
		if (percentageNumPeople < BUS_LOW_OCCUPANCY){
			busPixmap = QPixmap(":/resources/busA2Red.png");
		} else if (percentageNumPeople < BUS_HIGH_OCCUPANCY){
			busPixmap = QPixmap(":/resources/busA2Red.png");
		} else{
			busPixmap = QPixmap(":/resources/busA2Red.png");
		}
	} else if (busService == "B"){
		if (percentageNumPeople < BUS_LOW_OCCUPANCY){
			busPixmap = QPixmap(":/resources/busBGreen.png");
		} else if (percentageNumPeople < BUS_HIGH_OCCUPANCY){
			busPixmap = QPixmap(":/resources/busBGreen.png");
		} else{
			busPixmap = QPixmap(":/resources/busBGreen.png");
		}
	} else if (busService == "C"){
		if (percentageNumPeople < BUS_LOW_OCCUPANCY){
			busPixmap = QPixmap(":/resources/busCYellow.png");
		} else if (percentageNumPeople < BUS_HIGH_OCCUPANCY){
			busPixmap = QPixmap(":/resources/busCYellow.png");
		} else{
			busPixmap = QPixmap(":/resources/busCYellow.png");
		}
	} else if (busService == "D1"){
		if (percentageNumPeople < BUS_LOW_OCCUPANCY){
			busPixmap = QPixmap(":/resources/busD1.png");
		} else if (percentageNumPeople < BUS_HIGH_OCCUPANCY){
			busPixmap = QPixmap(":/resources/busD1.png");
		} else{
			busPixmap = QPixmap(":/resources/busD1.png");
		}
	} else if (busService == "D2"){
		if (percentageNumPeople < BUS_LOW_OCCUPANCY){
			busPixmap = QPixmap(":/resources/busD2.png");
		} else if (percentageNumPeople < BUS_HIGH_OCCUPANCY){
			busPixmap = QPixmap(":/resources/busD2.png");
		} else{
			busPixmap = QPixmap(":/resources/busD2.png");
		}
	} 

	busPixmap = busPixmap.scaledToHeight(30, Qt::SmoothTransformation);	
  	QGraphicsPixmapItem* busGraphics = new QGraphicsPixmapItem(busPixmap);
	busGraphics->setOffset(offsetx, offsety);
    mapScene.addItem(busGraphics);
}


