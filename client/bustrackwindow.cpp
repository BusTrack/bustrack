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
    
    BusStop tempStop;
    tempStop.setId("Qweq");
    tempStop.setName("bobabobo");
    tempStop.setOccupancy(10);
    tempStop.setLatitude(1.297970);
    tempStop.setLongitude(103.770000);
    busStopListComplete.append(tempStop);
    drawStop(0);

    Bus tempBus;
    tempBus.setId("ASDF");
    BusService tempService;
    tempService.setCode("A1");
    tempBus.setService(tempService);
    tempBus.setOccupancy(35);
    tempBus.setLatitude(1.297970);
    tempBus.setLongitude(103.772000);
    busListComplete.append(tempBus);
    drawBus(0);

/*
    drawBus("D1",1.297970,103.770000,50);
    drawBus("A1",1.297974,103.772000,30);
    drawBus("A1",1.297974,103.773000,20);
    drawBus("D2",1.297973,103.775000,10);
    drawBus("D2",1.297973,103.774000,0);
*/

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
        qDebug("%s: Found bus stop: %s", TAG.c_str(),bus_stop.getName().c_str());
        busStopListComplete.append(bus_stop);
        busStopList->addItem(bus_stop.getName().c_str());
    }
    for(int i=0; i<busStopListComplete.length(); i++)
    {
        drawStop(i);
    }
}

void BusTrackWindow::btsGetBusesComplete(std::vector<Bus> buses)
{
    for (Bus bus : buses) {
        qDebug("%s: Found bus: %s", TAG.c_str(), bus.getId().c_str());
        busListComplete.append(bus);
        busList->addItem(bus.getId().c_str());
    }
    for(int i=0; i<busListComplete.length(); i++)
    {
        drawBus(i);
    }
}

void BusTrackWindow::initializeLists()
{
    for(int i=0; i<BUS_NUM; i++)
    {
        busSelectedList.append(false);
    }

    dispatchList.append(ui->dispatchA1Btn);
    dispatchList.append(ui->dispatchA2Btn);
    dispatchList.append(ui->dispatchBBtn);
    dispatchList.append(ui->dispatchCBtn);
    dispatchList.append(ui->dispatchD1Btn);
    dispatchList.append(ui->dispatchD2Btn);

    busServiceList.append("A1");
    busServiceList.append("A2");
    busServiceList.append("B");
    busServiceList.append("C");
    busServiceList.append("D1");
    busServiceList.append("D2");
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
    if (searchActive && event->button() == Qt::RightButton)
        resetSearch();

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
        searchResultsWidget->setVisible(true);
        runSearch(query);
    }
    else
    {
        searchResultsWidget->setVisible(false);
        resetSearch();
    }
}

void BusTrackWindow::onBusInfoBtnClicked()
{

    if(!busInfoBtnClicked)
    {
        ui->busInfoBtn->setIcon(QIcon(":/resources/searchBusSel.png"));
        ui->stopInfoBtn->setIcon(QIcon(":/resources/searchStop.png"));
        busListWidget->setVisible(true);
        busStopListWidget->setVisible(false);
        busInfoBtnClicked = true;
        stopInfoBtnClicked = false;
    }
    else
    {
        ui->busInfoBtn->setIcon(QIcon(":/resources/searchBus.png"));
        busInfoBtnClicked = false;
        busListWidget->setVisible(false);
    }

}

void BusTrackWindow::onStopInfoBtnClicked()
{
    if(!stopInfoBtnClicked)
    {
        ui->stopInfoBtn->setIcon(QIcon(":/resources/searchStopSel.png"));
        ui->busInfoBtn->setIcon(QIcon(":/resources/searchBus.png"));
        busStopListWidget->setVisible(true);
        busListWidget->setVisible(false);
        stopInfoBtnClicked = true;
        busInfoBtnClicked = false;
    }
    else
    {
        ui->stopInfoBtn->setIcon(QIcon(":/resources/searchStop.png"));
        busStopListWidget->setVisible(false);
        stopInfoBtnClicked = false;
    }
}

void BusTrackWindow::toggleElementsVisibility()
{
    if(!hidden)
    {
        busListWidget->setVisible(false);
        busStopListWidget->setVisible(false);
        ui->zoomSliderWidget->setVisible(false);
        searchResultsWidget->setVisible(false);
        ui->searchBarWidget->setVisible(false);
        ui->sideBarWidget->setVisible(false);
        ui->dispatchWidget->setVisible(false);
        hidden = true;
    }
    else
    {
        busListWidget->setVisible(true);
        busStopListWidget->setVisible(true);
        ui->zoomSliderWidget->setVisible(true);
        searchResultsWidget->setVisible(true);
        ui->searchBarWidget->setVisible(true);
        ui->sideBarWidget->setVisible(true);
        ui->dispatchWidget->setVisible(true);
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

void BusTrackWindow::toggleDispatchButtons(int index)
{
    if(!busSelectedList[index])
    {
        busSelectedList[index] = true;
    }
    else
    {
        busSelectedList[index] = false;
    }
}

void BusTrackWindow::dispatchBus()
{
    QString buses = "Bus(es) ";
    for(int i=0; i<BUS_NUM; i++)
    {
        if(busSelectedList[i] == true)
        {
            // TODO dispatch bus
            qDebug() << busServiceList[i];
            buses.append(busServiceList[i] + ", ");
        }
    }
    statusBar()->showMessage(buses + "has/have been dispatched");
    ui->dispatchWidget->setVisible(false);
    cancelDispatch();
}

void BusTrackWindow::cancelDispatch()
{
    for(int i=0; i<BUS_NUM; i++)
    {
        busSelectedList[i] = false;
    }
    statusBar()->showMessage("No buses have been dispatched");
    ui->dispatchWidget->setVisible(false);
}

void BusTrackWindow::initializeWidgets()
{
    //qDebug() << "in intializeWidgets";
    searchResultsWidget = new QWidget(this);
    searchResultsWidget->setGeometry(79, 69, 350, 241);
    searchResultsList = new QListWidget(searchResultsWidget);
    searchResultsList->setGeometry(4, 4, 341, 232);
    searchResultsList->setGeometry(4, 4, 341, 242);
    searchResultsList->setViewMode(QListView::ListMode);
    searchResultsList->setFlow(QListView::TopToBottom);
    searchResultsList->setUniformItemSizes(true);
    searchResultsWidget->setVisible(false);

    busListWidget = new QWidget(this);
    busListWidget->setGeometry(80, 80, 350, 251);
    busList = new QListWidget(busListWidget);
    busList->setGeometry(4, 4, 341, 242);
    busList->setViewMode(QListView::ListMode);
    busList->setFlow(QListView::TopToBottom);
    busList->setUniformItemSizes(true);
    busListWidget->setVisible(false);

    busStopListWidget = new QWidget(this);
    busStopListWidget->setGeometry(80, 130, 350, 251);
    busStopList = new QListWidget(busStopListWidget);
    busStopList->setGeometry(4, 4, 341, 232);
    busStopList->setGeometry(4, 4, 341, 242);
    busStopList->setViewMode(QListView::ListMode);
    busStopList->setFlow(QListView::TopToBottom);
    busStopList->setUniformItemSizes(true);
    busStopListWidget->setVisible(false);

    ui->dispatchWidget->setVisible(false);

    setMap();

    hideAction = new QAction(tr("&Hide Elements"), this);
    hideAction->setShortcut(tr("Ctrl+H"));
    dispatchAction = new QAction(tr("Dispatch"), this);
    dispatchAction->setShortcut(tr("Ctrl+D"));

    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(hideAction);
    fileMenu->addAction(dispatchAction);
}

void BusTrackWindow::initializeValues()
{
    //qDebug() << "in initializeValues()";
    currentZoom = 1.0;
    slideValue = 1.0;
    busInfoBtnClicked = false;
    stopInfoBtnClicked = false;
    dispatchWidgetVisible = false;
    searchActive = false;
}

void BusTrackWindow::initializeConnections()
{
    //qDebug() << "in initializeConnections";
    connect(ui->zoomSlider, SIGNAL(valueChanged(int)), this, SLOT(zoomSlide(int)));
    connect(ui->searchLineEdit, SIGNAL(textChanged(QString)), this, SLOT(toggleSearchResultsWidget(QString)));
    connect(ui->busInfoBtn, SIGNAL(clicked()), this, SLOT(onBusInfoBtnClicked()));
    connect(ui->stopInfoBtn, SIGNAL(clicked()), this, SLOT(onStopInfoBtnClicked()));
    connect(hideAction, SIGNAL(triggered()), this, SLOT(toggleElementsVisibility()));
    connect(dispatchAction, SIGNAL(triggered()), this, SLOT(toggleDispatchWidget()));

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

    // connections for bus dispatch
    signalMapper = new QSignalMapper(this);
    for(int i=0; i<BUS_NUM; i++)
    {
        signalMapper->setMapping(dispatchList[i], i);
        connect(dispatchList[i], SIGNAL(clicked()), signalMapper, SLOT(map()));
    }
    connect(signalMapper, SIGNAL(mapped(int)), this, SLOT(toggleDispatchButtons(int)));

    connect(ui->dispatchButton, SIGNAL(clicked()), this, SLOT(dispatchBus()));
    connect(ui->cancelButton, SIGNAL(clicked()), this, SLOT(cancelDispatch()));
}

void BusTrackWindow::drawStop(int index)
{
    BusStop temp = busStopListComplete.at(index);
    float latitude = temp.getLatitude();
    float longitude = temp.getLongitude();
    QString name= QString::fromStdString(temp.getName());
    int numPeople = temp.getOccupancy();
    
    // 1.298037, 103.769591 (Top-left)
    // 1.292223, 103.780003 (Bottom-right)
    if (latitude > 1.298037 || longitude < 103.769591 ||
        latitude < 1.292223 || longitude > 103.780003)
        return;

    int offsety = 1080 * (1.298037-latitude)/(1.298037-1.292223);
    int offsetx = 1920 * (longitude-103.769591)/(103.780003-103.769591);

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

void BusTrackWindow::resetSearch()
{
    searchActive = false;
    searchOverlay->hide();
}

void BusTrackWindow::runSearch(QString query)
{
    resetSearch();
    qDebug() << query;
    bool resultsFound = false;
    QList<int> stopResultIndex;
    for (int i = 0 ; i < busStopListComplete.size(); i++) {
        BusStop curStop = busStopListComplete.at(i);
        qDebug() << QString::fromStdString(curStop.getName());
        if (QString::fromStdString(curStop.getName()).contains(query,Qt::CaseInsensitive)){
            resultsFound = true;
            stopResultIndex.append(i);
        }
    }

    QList<int> busResultIndex;
    for (int i = 0 ; i < busListComplete.size(); i++) {
        Bus curBus = busListComplete.at(i);
        qDebug() << QString::fromStdString(curBus.getService().getCode());
        if (QString::fromStdString(curBus.getService().getCode()).contains(query,Qt::CaseInsensitive)){
            resultsFound = true;
            busResultIndex.append(i);
        }
    }
    qDebug() << resultsFound;
    if (!resultsFound)
        return;

    searchActive = true;

    int radius = 50;
    QColor overlayColor;
    overlayColor.setRgb(1,1,1,100);
    searchOverlay = new QGraphicsPathItem(0);
    QPainterPath path1;
    path1.addRect(0,0,1920,1080);
    path1.setFillRule(Qt::OddEvenFill);
    searchOverlay->setBrush(overlayColor);
    searchOverlay->setOpacity(1);
    mapScene.addItem(searchOverlay);

    for (int i = 0; i < stopResultIndex.size(); i++){
        BusStop temp = busStopListComplete.at(stopResultIndex.at(i));
        float latitude = temp.getLatitude();
        float longitude = temp.getLongitude();
        // 1.298037, 103.769591 (Top-left)
        // 1.292223, 103.780003 (Bottom-right)
        if (latitude > 1.298037 || longitude < 103.769591 ||
            latitude < 1.292223 || longitude > 103.780003)
            continue;
        int offsety = 1080 * (1.298037-latitude)/(1.298037-1.292223);
        int offsetx = 1920 * (longitude-103.769591)/(103.780003-103.769591);
        path1.addEllipse(offsetx-radius+15, offsety-radius+17, 2*radius, 2*radius);
    }

    for (int i = 0; i < busResultIndex.size(); i++){
        Bus temp = busListComplete.at(busResultIndex.at(i));
        float latitude = temp.getLatitude();
        float longitude = temp.getLongitude();
        // 1.298037, 103.769591 (Top-left)
        // 1.292223, 103.780003 (Bottom-right)
        if (latitude > 1.298037 || longitude < 103.769591 ||
            latitude < 1.292223 || longitude > 103.780003)
            continue;
        int offsety = 1080 * (1.298037-latitude)/(1.298037-1.292223);
        int offsetx = 1920 * (longitude-103.769591)/(103.780003-103.769591);
        path1.addEllipse(offsetx-radius+33, offsety-radius+25, 2*radius, 2*radius);
    }
    searchOverlay->setPath(path1);
}

void BusTrackWindow::drawBus(int index)
{
    Bus temp = busListComplete.at(index);
    float latitude = temp.getLatitude();
    float longitude = temp.getLongitude();
    int numPeople = temp.getOccupancy();
    QString busService = QString::fromStdString(temp.getService().getCode());

    // 1.298037, 103.769591 (Top-left)
    // 1.292223, 103.780003 (Bottom-right)
    if (latitude > 1.298037 || longitude < 103.769591 ||
        latitude < 1.292223 || longitude > 103.780003)
        return;

    int offsety = 1080 * (1.298037-latitude)/(1.298037-1.292223);
    int offsetx = 1920 * (longitude-103.769591)/(103.780003-103.769591);

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
