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
    initializeWidgets();
    initializeConnections();
    initializeValues();

    ui->infoList->addItem("hello");
    ui->infoList->addItem("fawef");
    ui->infoList->addItem("auer");

    setMouseTracking(true);

    drawStop("Stop1",100,100,0);
    drawStop("Stop2",800,900,1);
    drawStop("Stop3",1000,300,15);
    
    drawBus("A1", 500, 400, 60);
    drawBus("B", 300, 400, 59);
    drawBus("A2", 200, 400, 30);
    drawBus("C", 100, 400, 2);
    drawBus("D2", 10, 400, 0);

    searchStop("Stop1");
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
        QList<QGraphicsItem *> childList = item->childItems();
        for (int i = 0; i < childList.size(); i++){
            if (childList[i]->isVisible()){
                childList[i]->hide();
            } else {
                childList[i]->show();
            }
        }
    }
    searchOverlay->hide();
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

void BusTrackWindow::toggleElementsVisibility()
{
    if(!hidden)
    {
        ui->infoListWidget->setVisible(false);
        ui->zoomSliderWidget->setVisible(false);
        ui->searchResultsWidget->setVisible(false);
        ui->searchBarWidget->setVisible(false);
        ui->sideBarWidget->setVisible(false);
        hidden = true;
    }
    else
    {
        ui->infoListWidget->setVisible(true);
        ui->zoomSliderWidget->setVisible(true);
        ui->searchResultsWidget->setVisible(true);
        ui->searchBarWidget->setVisible(true);
        ui->sideBarWidget->setVisible(true);
        hidden = false;
    }
}

void BusTrackWindow::initializeWidgets()
{
    ui->searchResultsWidget->setVisible(false);
    ui->infoListWidget->setVisible(false);
    setMap();

    hideAction = new QAction(tr("&Hide Elements"), this);
    hideAction->setShortcut(tr("Ctrl+H"));

    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(hideAction);
}

void BusTrackWindow::initializeConnections()
{
    connect(ui->zoomSlider, SIGNAL(valueChanged(int)), this, SLOT(zoomSlide(int)));
    connect(ui->searchLineEdit, SIGNAL(textChanged(QString)), this, SLOT(toggleSearchResultsWidget(QString)));
    connect(ui->busInfoBtn, SIGNAL(clicked()), this, SLOT(onBusInfoBtnClicked()));
    connect(ui->stopInfoBtn, SIGNAL(clicked()), this, SLOT(onStopInfoBtnClicked()));
    connect(hideAction, SIGNAL(triggered()), this, SLOT(toggleElementsVisibility()));
}

void BusTrackWindow::initializeValues()
{
    currentZoom = 1.0;
    slideValue = 1.0;
    busInfoBtnClicked = false;
    stopInfoBtnClicked = false;
    hidden = false; 
    searchActive = false;
}

void BusTrackWindow::drawStop(QString name, int offsetx, int offsety, int numPeople)
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

    //Locating pixels to be painted and replacing them with new color (busstop)
    QImage busstop;
    QImage world(2359, 1738, QImage::Format_RGB32);
    QSize sizeImage;
    int r,g,b,alpha;
    QRgb color;
    int height, width;
    busstop.load(":/resources/stopA.png");
    world.fill(1);	 
    QPainter painter(&world);
    sizeImage = busstop.size();
    width = sizeImage.width();
    height = sizeImage.height();	 
    for(int y = 0; y < height; y++) {
        for(int x = 0; x < width; x++) {
            color = busstop.pixel(x,y);
            if (qRed(color) == 255 && qGreen(color) == 0 && qBlue(color) == 36) {
         	  busstop.setPixel(x,y,repaintColor.rgb());
   	    }
        }
    }
    painter.drawImage(0,0,busstop);

    //Conversion of busstop image to pixmap and painting
    QPixmap busstopPixmap;
    busstopPixmap.convertFromImage(busstop);
    busstopPixmap = busstopPixmap.scaledToHeight(30, Qt::SmoothTransformation);
    QGraphicsPixmapItem* busstopGraphics = new QGraphicsPixmapItem(busstopPixmap);
    busstopGraphics->setOffset(offsetx, offsety);
    mapScene.addItem(busstopGraphics);
    busstopHash.insert(name, busstopGraphics);

    //Generation of child graphics (toggle-able additional info)
    QGraphicsRectItem* busstopinfoGraphics = new QGraphicsRectItem(offsetx+40, offsety-25, 150, 180);
    QColor windowBorderColor;
    windowBorderColor.setRgb(43,61,83);
    QPen windowPen;
    windowPen.setColor(windowBorderColor);
    windowPen.setWidth(4);
    busstopinfoGraphics->hide();
    busstopinfoGraphics->setParentItem(busstopGraphics);
    busstopinfoGraphics->setPen(windowPen);
    busstopinfoGraphics->setBrush(Qt::white);
    QFont boldFont;
    boldFont.setBold(true);
    QGraphicsSimpleTextItem* busstopName = new QGraphicsSimpleTextItem(name);
    busstopName->setFont(boldFont);
    busstopName->setPos(offsetx+50,offsety-15);
    busstopName->setParentItem(busstopGraphics);
    busstopName->hide();
    QGraphicsSimpleTextItem* busstopHeader = new QGraphicsSimpleTextItem("Bus        1st        2nd");
    busstopHeader->setFont(boldFont);
    busstopHeader->setPos(offsetx+50,offsety+5);
    busstopHeader->setParentItem(busstopGraphics);
    busstopHeader->hide();

    //Generation of busstopoccupany portion
    //Locating pixels to be painted and replacing them with new color (busstop occupancy)
    QImage busstopoccupancy;
    busstopoccupancy.load(":/resources/occupancy.png");
    QImage world2(2359, 1738, QImage::Format_RGB32);
    world2.fill(1);	 
    QPainter painter2(&world2);
    sizeImage = busstopoccupancy.size();
    width = sizeImage.width();
    height = sizeImage.height();	 
    for(int y = 0; y < height; y++) {
        for(int x = 0; x < width; x++) {
            color = busstopoccupancy.pixel(x,y);
            if (qRed(color) == 255 && qGreen(color) == 0 && qBlue(color) == 36) {
         	  busstopoccupancy.setPixel(x,y,repaintColor.rgb());
   	    }
        }
    }
    painter2.drawImage(0,0,busstopoccupancy);

    //Conversion of busstop occupancy image to pixmap and painting
    QPixmap busstopoccupancyPixmap;
    busstopoccupancyPixmap.convertFromImage(busstopoccupancy);
    busstopoccupancyPixmap = busstopoccupancyPixmap.scaledToHeight(60, Qt::SmoothTransformation);
    QGraphicsPixmapItem* busstopoccupancyGraphics = new QGraphicsPixmapItem(busstopoccupancyPixmap);
    busstopoccupancyGraphics->setParentItem(busstopGraphics);
    busstopoccupancyGraphics->hide();
    busstopoccupancyGraphics->setOffset(offsetx-30, offsety+30);
    //Busstop occupancy
    QGraphicsSimpleTextItem* busstopoccupancyNumber = new QGraphicsSimpleTextItem(QString::number(numPeople));
    busstopoccupancyNumber-> setPos(offsetx+5,offsety+47);
    busstopoccupancyNumber->setParentItem(busstopGraphics);
    busstopoccupancyNumber->setBrush(Qt::white);
    busstopoccupancyNumber->hide();
}

void BusTrackWindow::searchStop(QString name)
{
    searchActive = true;
    int offsetx = 1000, offsety = 300, radius = 50;

    QColor overlayColor;
    overlayColor.setRgb(1,1,1,100);
    searchOverlay = new QGraphicsPathItem(0);
    QPainterPath path1;
    path1.addRect(0,0,1920,1080);
    path1.addEllipse(offsetx-radius+15, offsety-radius+17, 2*radius, 2*radius);
    path1.addEllipse(offsetx-radius+90, offsety-radius+90, 2*radius, 2*radius);
    path1.setFillRule(Qt::OddEvenFill);
    searchOverlay->setBrush(overlayColor);
    searchOverlay->setOpacity(1);
    searchOverlay->setPath(path1);
    mapScene.addItem(searchOverlay);
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
	busPixmap = busPixmap.scaledToHeight(50, Qt::SmoothTransformation);
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
