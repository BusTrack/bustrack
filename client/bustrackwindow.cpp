#include "bustrackservice.h"
#include "bustrackwindow.h"
#include "ui_bustrackwindow.h"

const float ZOOM_FACTOR = 1.2;
const float MIN_ZOOM = 1.0;
const float MAX_ZOOM = 6.0;
const float MAX_NUM_PEOPLE_BUS = 50;
const float BUS_NUM = 6;

namespace bustrack {

const std::string BusTrackWindow::TAG ("BusTrackWindow");

BusTrackWindow::BusTrackWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::BusTrackWindow)
{
    // Initialize the BusTrack service.
    busTrackService = new bustrack::BusTrackService();

    ui->setupUi(this);

    initializeLists();
    initializeWidgets();
    initializeValues();
    initializeConnections();

    setMouseTracking(true);

    drawBus("D1",1.297970,103.770000,50);
    drawBus("A1",1.297974,103.772000,30);
    drawBus("A1",1.297974,103.773000,20);
    drawBus("D2",1.297973,103.775000,10);
    drawBus("D2",1.297973,103.774000,0);
}

BusTrackWindow::~BusTrackWindow()
{
    delete ui;
}

// event handlers
void BusTrackWindow::btsConnected()
{
    busTrackService->getBusStops();
    busTrackService->getBuses();
}

void BusTrackWindow::btsGetBusStopsComplete(std::vector<BusStop> bus_stops)
{
    for (BusStop bus_stop : bus_stops) {
        qDebug("%s: Found bus stop: %s", TAG.c_str(),
            bus_stop.getName().c_str());
        busStopListComplete.append(bus_stop.getName().c_str());

        drawStop(QString(bus_stop.getName().c_str()), bus_stop.getLatitude(),
                bus_stop.getLongitude(), bus_stop.getOccupancy());
    }

    for(int i=0; i<busStopListComplete.length(); i++)
    {
        ui->busStopList->addItem(busStopListComplete[i]);
    }
}

void BusTrackWindow::btsGetBusesComplete(std::vector<Bus> buses)
{
    for (Bus bus : buses) {
        qDebug("%s: Found bus: %s", TAG.c_str(), bus.getId().c_str());
        busListComplete.append(bus.getId().c_str());
    }

    for(int i=0; i<busListComplete.length(); i++)
    {
        ui->busInfoList->addItem(busListComplete[i]);
    }
}

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
    if (searchActive) {
        searchOverlay->hide();
        searchActive = false;
        return;
    }

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
    map = QPixmap(":/resources/map.png");
    mapPixmap = new QGraphicsPixmapItem(map);
    mapPixmap->setTransformationMode(Qt::SmoothTransformation);
    mapScene.addItem(mapPixmap);
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
        //ui->infoListWidget->move(79, 80);
        ui->busInfoListWidget->setVisible(true);
        busInfoBtnClicked = true;
    }
    else
    {
        ui->busInfoBtn->setIcon(QIcon(":/resources/searchBus.png"));
        busInfoBtnClicked = false;
        ui->busInfoListWidget->setVisible(false);
    }

}

void BusTrackWindow::onStopInfoBtnClicked()
{
    if(!stopInfoBtnClicked)
    {
        ui->stopInfoBtn->setIcon(QIcon(":/resources/searchStopSel.png"));
        ui->busInfoBtn->setIcon(QIcon(":/resources/searchBus.png"));
        //ui->infoListWidget->move(79, 140);
        ui->busStopInfoListWidget->setVisible(true);
        stopInfoBtnClicked = true;
    }
    else
    {
        ui->stopInfoBtn->setIcon(QIcon(":/resources/searchStop.png"));
        ui->busStopInfoListWidget->setVisible(false);
        stopInfoBtnClicked = false;

    }
}

void BusTrackWindow::toggleElementsVisibility()
{
    if(!hidden)
    {
        ui->busInfoListWidget->setVisible(false);
        ui->busStopInfoListWidget->setVisible(false);
        ui->zoomSliderWidget->setVisible(false);
        ui->searchResultsWidget->setVisible(false);
        ui->searchBarWidget->setVisible(false);
        ui->sideBarWidget->setVisible(false);
        ui->dispatchWidget->setVisible(false);
        ui->calendarWidget->setVisible(false);
        ui->timeEdit->setVisible(false);
        hidden = true;
    }
    else
    {
        ui->busInfoListWidget->setVisible(true);
        ui->busStopInfoListWidget->setVisible(true);
        ui->zoomSliderWidget->setVisible(true);
        ui->searchResultsWidget->setVisible(true);
        ui->searchBarWidget->setVisible(true);
        ui->sideBarWidget->setVisible(true);
        ui->dispatchWidget->setVisible(true);
        ui->calendarWidget->setVisible(true);
        ui->timeEdit->setVisible(true);
        hidden = false;
    }
}

void BusTrackWindow::toggleDispatchWidget()
{
    if(!dispatchWidgetVisible)
    {
        ui->dispatchWidget->setVisible(true);
        dispatchWidgetVisible = true;
    }
    else
    {
        ui->dispatchWidget->setVisible(false);
        dispatchWidgetVisible = false;
    }
}

void BusTrackWindow::createCalendarWidget()
{
    ui->calendarWidget->move(QCursor::pos()-QPoint(30, 30));
    ui->calendarWidget->setVisible(true);
}

void BusTrackWindow::createTimeWidget()
{
    ui->timeEdit->move(QCursor::pos()-QPoint(30, 30));
    ui->timeEdit->setVisible(true);
}

void BusTrackWindow::calendarSelectionChanged(int i)
{

}

void BusTrackWindow::timeSelectionChanged(int i)
{
}

void BusTrackWindow::initializeLists()
{
    calButtonList.append(ui->A1CalButton);
    calButtonList.append(ui->A2CalButton);
    calButtonList.append(ui->BCalButton);
    calButtonList.append(ui->CCalButton);
    calButtonList.append(ui->D1CalButton);
    calButtonList.append(ui->D2CalButton);

    clockButtonList.append(ui->A1ClockButton);
    clockButtonList.append(ui->A2ClockButton);
    clockButtonList.append(ui->BClockButton);
    clockButtonList.append(ui->CClockButton);
    clockButtonList.append(ui->D1ClockButton);
    clockButtonList.append(ui->D2ClockButton);

    calTickList.append(ui->A1Tick);
    calTickList.append(ui->A2Tick);
    calTickList.append(ui->BTick);
    calTickList.append(ui->CTick);
    calTickList.append(ui->D1Tick);
    calTickList.append(ui->D2Tick);

    clockTickList.append(ui->A1Tick2);
    clockTickList.append(ui->A2Tick2);
    clockTickList.append(ui->BTick2);
    clockTickList.append(ui->CTick2);
    clockTickList.append(ui->D1Tick2);
    clockTickList.append(ui->D2Tick2);
}

void BusTrackWindow::initializeWidgets()
{
    ui->zoomSliderWidget->setVisible(true);
    ui->searchBarWidget->setVisible(true);
    ui->sideBarWidget->setVisible(true);

    ui->searchResultsWidget->setVisible(false);
    ui->busInfoListWidget->setVisible(false);
    ui->busStopInfoListWidget->setVisible(false);
    ui->dispatchWidget->setVisible(false);
    ui->calendarWidget->setVisible(false);
    ui->timeEdit->setVisible(false);

    for(int i=0; i<BUS_NUM; i++)
    {
        calTickList[i]->setVisible(false);
        clockTickList[i]->setVisible(false);
    }

    setMap();

    hideAction = new QAction(tr("&Hide Elements"), this);
    hideAction->setShortcut(tr("Ctrl+H"));
    dispatchAction = new QAction(tr("Dispatch"), this);
    dispatchAction->setShortcut(tr("Ctrl+D"));

    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(hideAction);
    fileMenu->addAction(dispatchAction);

    ui->busInfoListWidget->move(79, 80);
    ui->busStopInfoListWidget->move(79, 140);
}

void BusTrackWindow::initializeValues()
{
    currentZoom = 1.0;
    slideValue = 1.0;
    busInfoBtnClicked = false;
    stopInfoBtnClicked = false;
    dispatchWidgetVisible = false;
    searchActive = false;
}

void BusTrackWindow::initializeConnections()
{
    connect(ui->zoomSlider, SIGNAL(valueChanged(int)), this, SLOT(zoomSlide(int)));
    connect(ui->searchLineEdit, SIGNAL(textChanged(QString)), this, SLOT(toggleSearchResultsWidget(QString)));
    connect(ui->busInfoBtn, SIGNAL(clicked()), this, SLOT(onBusInfoBtnClicked()));
    connect(ui->stopInfoBtn, SIGNAL(clicked()), this, SLOT(onStopInfoBtnClicked()));
    connect(hideAction, SIGNAL(triggered()), this, SLOT(toggleElementsVisibility()));
    connect(dispatchAction, SIGNAL(triggered()), this, SLOT(toggleDispatchWidget()));

    signalMapper = new QSignalMapper(this);
    for(int i=0; i<BUS_NUM; i++)
    {
        signalMapper->setMapping(calButtonList[i], calButtonList[i]);
        connect(calButtonList[i], SIGNAL(clicked()), signalMapper, SLOT(map()));
    }
    connect(signalMapper, SIGNAL(mapped(QWidget*)), this, SLOT(createCalendarWidget()));

    signalMapper = new QSignalMapper(this);
    for(int i=0; i<BUS_NUM; i++)
    {
        signalMapper->setMapping(clockButtonList[i], clockButtonList[i]);
        connect(clockButtonList[i], SIGNAL(clicked()), signalMapper, SLOT(map()));
    }
    connect(signalMapper, SIGNAL(mapped(QWidget*)), this, SLOT(createTimeWidget()));

    // Receive events from BusTrackService.
    connect(busTrackService, SIGNAL(connected()), this, SLOT(btsConnected()));
    connect(
        busTrackService,
        SIGNAL(getBusStopsComplete(std::vector<BusStop>)),
        this,
        SLOT(btsGetBusStopsComplete(std::vector<BusStop>))
    );
    connect(
        busTrackService,
        SIGNAL(getBusesComplete(std::vector<Bus>)),
        this,
        SLOT(btsGetBusesComplete(std::vector<Bus>))
    );
}

void BusTrackWindow::drawStop(QString name, float latitude, float longitude, int numPeople)
{
    // 1.298037, 103.769591 (Top-left)
    // 1.292223, 103.780003 (Bottom-right)
    if (latitude > 1.298037 || longitude < 103.769591 ||
        latitude < 1.292223 || longitude > 103.780003)
        return;

    int offsetx = 1080 * (latitude-1.292223)/(1.298037-1.292223);
    int offsety = 1920 * (longitude-103.769591)/(103.780003-103.769591);

    //Color transitioning from red to yellow to green
    QColor repaintColor;
    repaintColor.setRgb(255,0,36);
    if (numPeople < 30 && numPeople > 15) {
        repaintColor.setRgb(255 + (240-255)/15*(numPeople-15), 0 + 240/15*(numPeople-15), 36);
    } else if (numPeople > 0 && numPeople <= 15) {
        repaintColor.setRgb(0 +  240.0/15*numPeople, 210.0 + 30/15*numPeople, 36);
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
    busstopGraphics->setTransformationMode(Qt::SmoothTransformation);
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
    QImage world2(400, 400, QImage::Format_RGB32);
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
    busstopoccupancyGraphics->setTransformationMode(Qt::SmoothTransformation);
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
    path1.setFillRule(Qt::OddEvenFill);
    searchOverlay->setBrush(overlayColor);
    searchOverlay->setOpacity(1);
    searchOverlay->setPath(path1);
    mapScene.addItem(searchOverlay);
}

void BusTrackWindow::searchBus(QString name)
{
    searchActive = true;


    int offsetx = 1000, offsety = 300, radius = 50;

    QColor overlayColor;
    overlayColor.setRgb(1,1,1,100);
    searchOverlay = new QGraphicsPathItem(0);
    QPainterPath path1;
    path1.addRect(0,0,1920,1080);
    path1.addEllipse(offsetx-radius+15, offsety-radius+17, 2*radius, 2*radius);
    path1.setFillRule(Qt::OddEvenFill);
    searchOverlay->setBrush(overlayColor);
    searchOverlay->setOpacity(1);
    searchOverlay->setPath(path1);
    mapScene.addItem(searchOverlay);
}

void BusTrackWindow::drawBus(QString busService, float latitude, float longitude, int numPeople)
{
    // 1.298037, 103.769591 (Top-left)
    // 1.292223, 103.780003 (Bottom-right)
    if (latitude > 1.298037 || longitude < 103.769591 ||
        latitude < 1.292223 || longitude > 103.780003)
        return;

    int offsetx = 1080 * (latitude-1.292223)/(1.298037-1.292223);
    int offsety = 1920 * (longitude-103.769591)/(103.780003-103.769591);

	float percentageNumPeople = (numPeople * 1.0)/MAX_NUM_PEOPLE_BUS;
    //Color transitioning from red to yellow to green
    QColor repaintBusColor;
    repaintBusColor.setRgb(255,0,36);
    if (numPeople < 50 && numPeople > 25) {
        repaintBusColor.setRgb(255 + (240-255)/25*(numPeople-25), 0 + 240/25*(numPeople-25),36);
    } else if (numPeople > 0 && numPeople <= 25) {
        repaintBusColor.setRgb(0 +  240.0/25*numPeople, 210.0 + 30/25*numPeople, 36);
    } else if (numPeople == 0) {
        repaintBusColor.setRgb(0,210,36);
    }

	//Locating pixels to be painted and replacing them with new color
	QImage world(400, 400, QImage::Format_RGB32);
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
    busGraphics->setTransformationMode(Qt::SmoothTransformation);
	busGraphics->setOffset(offsetx, offsety);
   	mapScene.addItem(busGraphics);


    QImage busoccupancy;
    busoccupancy.load(":/resources/occupancy.png");
    QImage world2(400, 400, QImage::Format_RGB32);
    world2.fill(1);	 
    QPainter painter2(&world2);
    sizeImage = busoccupancy.size();
    width = sizeImage.width();
    height = sizeImage.height();	 
    for(int y = 0; y < height; y++) {
        for(int x = 0; x < width; x++) {
            color = busoccupancy.pixel(x,y);
            if (qRed(color) == 255 && qGreen(color) == 0 && qBlue(color) == 36) {
         	  busoccupancy.setPixel(x,y,repaintBusColor.rgb());
   	    }
        }
    }
    painter2.drawImage(0,0,busoccupancy);
    QPixmap busoccupancyPixmap;
    busoccupancyPixmap.convertFromImage(busoccupancy);
    busoccupancyPixmap = busoccupancyPixmap.scaledToHeight(60, Qt::SmoothTransformation);
    QGraphicsPixmapItem* busoccupancyGraphics = new QGraphicsPixmapItem(busoccupancyPixmap);
    busoccupancyGraphics->setTransformationMode(Qt::SmoothTransformation);
    busoccupancyGraphics->setParentItem(busGraphics);
    busoccupancyGraphics->hide();
    busoccupancyGraphics->setOffset(offsetx-30, offsety+30);
    //Busstop occupancy
    QGraphicsSimpleTextItem* busoccupancyNumber = new QGraphicsSimpleTextItem(QString::number(numPeople*100/MAX_NUM_PEOPLE_BUS));
    busoccupancyNumber-> setPos(offsetx+5,offsety+47);
    busoccupancyNumber->setParentItem(busGraphics);
    busoccupancyNumber->setBrush(Qt::white);
    busoccupancyNumber->hide();

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

}

// vim: set ts=4 sw=4 et:
