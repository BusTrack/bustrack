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
    ui(new Ui::BusTrackWindow),
    initializedBusStops(false),
    initializedBusServices(false),
    initializedBusStopServices(false)
{
    // Initialize the BusTrack service.
    busTrackService = new bustrack::BusTrackService();

    ui->setupUi(this);

    initializeLists();
    initializeWidgets();
    initializeValues();
    initializeConnections();
}

BusTrackWindow::~BusTrackWindow()
{
    delete ui;
}

// event handlers
void BusTrackWindow::btsConnected()
{
    busTrackService->getBuses();
}

void BusTrackWindow::btsGetBusServicesComplete(
        std::vector<BusService> bus_services)
{
    busServiceListComplete.clear();
    for (BusService bus_service : bus_services) {
        qDebug("%s: Found bus service: %s", TAG.c_str(),
                bus_service.getCode().c_str());
        busServiceListComplete.append(bus_service);
    }

    initializedBusServices = true;
    initializeBusStopServices();
    
    busTrackService->getBusStops();
}

void BusTrackWindow::btsGetBusStopsComplete(std::vector<BusStop> bus_stops)
{
    busStopListComplete.clear();
    for (BusStop bus_stop : bus_stops) {
        qDebug("%s: Found bus stop: %s", TAG.c_str(),bus_stop.getName().c_str());
        busStopListComplete.append(bus_stop);
        busStopList->addItem(bus_stop.getName().c_str());
    }

    initializedBusStops = true;
    initializeBusStopServices();

    updateMap();
    
}

void BusTrackWindow::btsGetBusesComplete(std::vector<Bus> buses)
{
    busListComplete.clear();
    for (Bus bus : buses) {
        qDebug("%s: Found bus: %s (%f, %f)", TAG.c_str(), bus.getId().c_str(),
                bus.getLatitude(), bus.getLongitude());
        busListComplete.append(bus);
    }
    
    busTrackService->getBusServices();
}

void BusTrackWindow::btsRefresh() {
    qDebug("%s: New data available from server. Refreshing...", TAG.c_str());
    busTrackService->getBuses();
}

void BusTrackWindow::updateMap()
{
    for (QGraphicsItem* item : paintedObjects) {
        mapScene.removeItem(item);
    }
    paintedObjects.clear();
    for(int i=0; i<busStopListComplete.length(); i++)
    {
        drawStop(i);
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

void BusTrackWindow::resizeEvent(QResizeEvent *event)
{
    QSize currentSize = size();
    ui->zoomSliderWidget->move(currentSize.width()*0.93, currentSize.height()*0.6);
    ui->dispatchWidget->move(currentSize.width()*0.12, currentSize.height()*0.55);
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
        searchResultsWidget->setVisible(false);
        runSearch(query);
    }
    else
    {
        searchResultsWidget->setVisible(false);
        resetSearch();
    }
}

void BusTrackWindow::setSearchTextFromBuses(QListWidgetItem* item){
    for (int i = 0; i < busList->count() ; i++){
        if (busList->item(i) == item){
            ui->searchLineEdit->setText(item->text());
        }
    }
}

void BusTrackWindow::setSearchTextFromBusStops(QListWidgetItem* item){
     for (int i = 0; i < busStopList->count() ; i++){
        if (busStopList->item(i) == item){
            ui->searchLineEdit->setText(item->text());
             
        }
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
    //testing updateMap() function
    updateMap();

    QString buses = "Bus(es) ";
    for(int i=0; i<BUS_NUM; i++)
    {
        if(busSelectedList[i] == true)
        {
            busTrackService->dispatchBus(busServiceList[i].toStdString());
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

    busList->addItem("A1");
    busList->addItem("A2");
    busList->addItem("B");
    busList->addItem("C");
    busList->addItem("D1");
    busList->addItem("D2");

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
    initializedBusStopServices = false;
    searchOverlay = new QGraphicsPathItem(0);
    searchServiceOverlay = new QGraphicsPathItem(0);
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
    
    connect(busList, SIGNAL(itemClicked(QListWidgetItem*)), 
        this, SLOT(setSearchTextFromBuses(QListWidgetItem*))); 
    connect(busStopList, SIGNAL(itemClicked(QListWidgetItem*)), 
        this, SLOT(setSearchTextFromBusStops(QListWidgetItem*)));   

    // Receive events from BusTrackService.
    connect(busTrackService, SIGNAL(connected()), this, SLOT(btsConnected()));
    connect(
        busTrackService,
        SIGNAL(getBusServicesComplete(std::vector<BusService>)),
        this,
        SLOT(btsGetBusServicesComplete(std::vector<BusService>))
    );
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

void BusTrackWindow::initializeBusStopServices()
{
    if (!initializedBusServices || !initializedBusStops ||
         initializedBusStopServices) {
        return;
    }

    initializedBusStopServices = true;
    int i = 0;
    for (BusStop bus_stop : busStopListComplete) {
        QList<BusService> tempList;
        for (BusService bus_service : busServiceListComplete) {
            std::vector<std::shared_ptr<Waypoint>> route = bus_service.getRoute();
            for (std::shared_ptr<Waypoint> waypoint : route) {
                std::shared_ptr<BusStop> waypoint_temp = std::dynamic_pointer_cast<BusStop>(waypoint);
                if (waypoint_temp->getId() == bus_stop.getId()) {
                    tempList.append(bus_service);
                }                
            }
        }
        busStopServices.insert(i, tempList);
        i++;
    }

    busTrackServiceTimer = new QTimer(this);
    connect(busTrackServiceTimer, SIGNAL(timeout()), this, SLOT(btsRefresh()));
    busTrackServiceTimer->start(15000);
}

void BusTrackWindow::drawStop(int index)
{
    BusStop temp = busStopListComplete.at(index);
    float latitude = temp.getLatitude();
    float longitude = temp.getLongitude();
    QString name= QString::fromStdString(temp.getName());
    int numPeople = temp.getOccupancy();
    QList<BusService> busServiceList = busStopServices.value(index);
    
    // 1.304444, 103.769694 (Top-left)
    // 1.290457, 103.786843 (Bottom-right)
    if (latitude > 1.304444 || longitude < 103.769694 ||
        latitude < 1.290457 || longitude > 103.786843)
        return;

    int offsety = 2617 * (1.304444-latitude)/(1.304444-1.290457);
    int offsetx = 3205 * (longitude-103.769694)/(103.786843-103.769694);

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
            if (qRed(color) == 0 && qGreen(color) == 0 && qBlue(color) == 0) {
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
    paintedObjects.append(busstopGraphics);

    //Generation of child graphics (toggle-able additional info)
    int rectHeight = 60 + 15*busServiceList.size();
    QGraphicsRectItem* busstopinfoGraphics = new QGraphicsRectItem(offsetx+40, offsety-25, 250, rectHeight);
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
    for (int i = 0; i < busServiceList.size(); i++) {
        BusService service = busServiceList.at(i);
        int first = 999, second = 999;
        for (Bus bus : busListComplete) {
            if (bus.getService().getCode() == service.getCode()) {
                int destinationIndex = bus.getDestination();
                int busStopIndex = 999999;
                for (int j = destinationIndex; j < service.getRoute().size() && busStopIndex == 999999; j++) {
                    if (latitude == service.getRoute().at(j)->getLatitude() &&
                            longitude == service.getRoute().at(j)->getLongitude()) {
                        busStopIndex = j;
                    }
                }
                int distance = 2 * (busStopIndex - destinationIndex + 1);
                if (distance < first) {
                    second = first;
                    first = distance;
                } else if (distance < second) {
                    second = distance;
                }
            }
        }
        QString stringFirst;
        QString stringSecond;
        if (first == 999)
            stringFirst = "NA";
        else
            stringFirst = QString::number(first);
        if (second == 999)
            stringSecond = "NA";
        else
            stringSecond = QString::number(second);
        QString serviceArrival = QString::fromStdString(service.getCode())+"           "+stringFirst+"            "+stringSecond;
        QGraphicsSimpleTextItem* busServiceArrivals = new QGraphicsSimpleTextItem(serviceArrival);
//        busServiceArrivals->setFont(boldFont);
        busServiceArrivals->setPos(offsetx+50,offsety+5+(15*(i+1)));
        busServiceArrivals->setParentItem(busstopGraphics);
        busServiceArrivals->hide();

    }

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
    searchServiceOverlay->hide();
}

void BusTrackWindow::runSearch(QString query)
{
    resetSearch();
    bool resultsFound = false;
    QList<int> stopResultIndex;
    for (int i = 0 ; i < busStopListComplete.size(); i++) {
        BusStop curStop = busStopListComplete.at(i);
        if (QString::fromStdString(curStop.getName()).contains(query,Qt::CaseInsensitive)){
            resultsFound = true;
            stopResultIndex.append(i);
        }
    }

    QList<int> busResultIndex;
    for (int i = 0 ; i < busListComplete.size(); i++) {
        Bus curBus = busListComplete.at(i);
        if (QString::fromStdString(curBus.getService().getCode()).contains(query,Qt::CaseInsensitive)){
            resultsFound = true;
            busResultIndex.append(i);
        }
    }
    if (!resultsFound)
        return;

    searchActive = true;

    int radius = 50;
    QColor overlayColor;
    overlayColor.setRgb(1,1,1,100);
    searchOverlay = new QGraphicsPathItem(0);
    QPainterPath path1;
    path1.addRect(0,0,3205,2617);
    path1.setFillRule(Qt::OddEvenFill);
    searchOverlay->setBrush(overlayColor);
    searchOverlay->setOpacity(1);
    mapScene.addItem(searchOverlay);

    for (int i = 0; i < stopResultIndex.size(); i++){
        BusStop temp = busStopListComplete.at(stopResultIndex.at(i));
        float latitude = temp.getLatitude();
        float longitude = temp.getLongitude();
    // 1.304444, 103.769694 (Top-left)
    // 1.290457, 103.786843 (Bottom-right)
    if (latitude > 1.304444 || longitude < 103.769694 ||
        latitude < 1.290457 || longitude > 103.786843)
        continue;

    int offsety = 2617 * (1.304444-latitude)/(1.304444-1.290457);
    int offsetx = 3205 * (longitude-103.769694)/(103.786843-103.769694);
        path1.addEllipse(offsetx-radius+15, offsety-radius+17, 2*radius, 2*radius);
    }

    for (int i = 0; i < busResultIndex.size(); i++){
        Bus temp = busListComplete.at(busResultIndex.at(i));
        float latitude = temp.getLatitude();
        float longitude = temp.getLongitude();
    // 1.304444, 103.769694 (Top-left)
    // 1.290457, 103.786843 (Bottom-right)
    if (latitude > 1.304444 || longitude < 103.769694 ||
        latitude < 1.290457 || longitude > 103.786843)
        continue;

    int offsety = 2617 * (1.304444-latitude)/(1.304444-1.290457);
    int offsetx = 3205 * (longitude-103.769694)/(103.786843-103.769694);
        path1.addEllipse(offsetx-radius+33, offsety-radius+25, 2*radius, 2*radius);
    }
    searchOverlay->setPath(path1);
    
    for (int i = 0; i < busServiceListComplete.size(); i++) {
        BusService curService= busServiceListComplete.at(i);
        if (QString::fromStdString(curService.getCode()) == query) {
            searchServiceOverlay = new QGraphicsPathItem(0);
            QPen windowPen;
            windowPen.setColor(Qt::cyan);
            windowPen.setWidth(4);
            searchServiceOverlay->setPen(windowPen);
            QPointF startPoint;
            startPoint.setX(3205 * (curService.getRoute().at(0)->getLongitude()-103.769694)/(103.786843-103.769694)+15);
            startPoint.setY(2617 * (1.304444-curService.getRoute().at(0)->getLatitude())/(1.304444-1.290457)+15);
            QPainterPath path(startPoint);
            std::vector<std::shared_ptr<Waypoint>> route = curService.getRoute();
            for (std::shared_ptr<Waypoint> waypoint : route) {
                QPointF nextPoint;
                nextPoint.setX(3205 * (waypoint->getLongitude()-103.769694)/(103.786843-103.769694) + 15);
                nextPoint.setY(2617 * (1.304444-waypoint->getLatitude())/(1.304444-1.290457) +15);
                path.lineTo(nextPoint);
            }
            searchServiceOverlay->setPath(path);
            mapScene.addItem(searchServiceOverlay);
        }
    }   
}

void BusTrackWindow::drawBus(int index)
{
    Bus temp = busListComplete.at(index);
    float latitude = temp.getLatitude();
    float longitude = temp.getLongitude();
    int numPeople = temp.getOccupancy();
    QString busService = QString::fromStdString(temp.getService().getCode());

    // 1.304444, 103.769694 (Top-left)
    // 1.290457, 103.786843 (Bottom-right)
    if (latitude > 1.304444 || longitude < 103.769694 ||
        latitude < 1.290457 || longitude > 103.786843)
        return;

    int offsety = 2617 * (1.304444-latitude)/(1.304444-1.290457);
    int offsetx = 3205 * (longitude-103.769694)/(103.786843-103.769694);
	float percentageNumPeople = (numPeople * 1.0)/MAX_NUM_PEOPLE_BUS;
    
    std::vector<std::shared_ptr<Waypoint>> tempRoute;
    for (BusService bus_service : busServiceListComplete) {
        if (bus_service.getCode() == temp.getService().getCode()) {
            tempRoute = bus_service.getRoute();
        }
    }

    std::shared_ptr<Waypoint> destinationWaypoint = tempRoute.at(temp.getDestination());
    int rotationDegree = qAtan((latitude-(destinationWaypoint->getLatitude()))/(longitude-(destinationWaypoint->getLongitude())))/3.142*180;
    QMatrix rotateMatrix;
    rotateMatrix.rotate(rotationDegree*-1 -145);


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
		    if (qRed(color) == 255 && qGreen(color) == 255 && qBlue(color) == 255) {
		 	  background.setPixel(x,y,repaintBusColor.rgb());
		    }
		}
	}
	painter.drawImage(0,0,background);

	//Conversion of image to pixmap and scale
	QPixmap busPixmap;
	busPixmap.convertFromImage(background);
	busPixmap = busPixmap.scaledToHeight(50, Qt::SmoothTransformation);
    busPixmap = busPixmap.transformed(rotateMatrix);
	QGraphicsPixmapItem* busGraphics = new QGraphicsPixmapItem(busPixmap);
    busGraphics->setTransformationMode(Qt::SmoothTransformation);
	busGraphics->setOffset(offsetx, offsety);
   	mapScene.addItem(busGraphics);
    paintedObjects.append(busGraphics);


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
