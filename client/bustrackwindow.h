#ifndef BUSTRACKWINDOW_H
#define BUSTRACKWINDOW_H

#include <QMainWindow>
#include <QWheelEvent>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QPixmap>
#include <QDebug>
#include <QStandardItemModel>
#include <QHoverEvent>
#include <QCalendarWidget>
#include <QDateTimeEdit>
#include <QSignalMapper>
#include <QList>
#include <QPushButton>
#include <QLabel>
#include <QListWidget>

#include "bustrack/bus.h"
#include "bustrack/bus_stop.h"

namespace Ui {
class BusTrackWindow;
}

namespace bustrack {
class BusTrackService;
}

namespace bustrack {

class BusTrackWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit BusTrackWindow(QWidget *parent = 0);
    ~BusTrackWindow();

protected:
    void wheelEvent(QWheelEvent *event);
    void resizeEvent(QResizeEvent *event);
    void mousePressEvent(QMouseEvent* event);
    void mouseMoveEvent(QMouseEvent* event);

signals:

private slots:
    void zoomSlide(int newZoom);
    void toggleSearchResultsWidget(QString query);
    void onBusInfoBtnClicked();
    void onStopInfoBtnClicked();
    void toggleElementsVisibility();
    void toggleDispatchWidget();
    void toggleDispatchButtons(int index);
    void dispatchBus();
    void cancelDispatch();

    void btsConnected();
    void btsGetBusStopsComplete(std::vector<BusStop> bus_stops);
    void btsGetBusesComplete(std::vector<Bus> buses);

private:
    static const std::string TAG;
    
    Ui::BusTrackWindow *ui;
    QGraphicsScene mapScene;
    QGraphicsPixmapItem* mapPixmap;
    QPixmap map;
    QMenu *fileMenu;
    QAction *hideAction;
    QAction *dispatchAction;
    bustrack::BusTrackService* busTrackService;

    float currentZoom;
    int slideValue;
    bool busInfoBtnClicked;
    bool stopInfoBtnClicked;
    bool hidden;
    bool dispatchWidgetVisible;

    QHash<QString,QGraphicsItem*> busstopHash;
    bool searchActive;
    QGraphicsPathItem* searchOverlay;
    QSignalMapper *signalMapper;
    QWidget *searchResultsWidget;
    QListWidget *searchResultsList;
    QWidget *busListWidget;
    QListWidget *busList;
    QWidget *busStopListWidget;
    QListWidget *busStopList;

    // for bus dispatch
    QList<QPushButton*> dispatchList;
    QList<bool> busSelectedList;
    QList<QString> busServiceList;

    // models equivalent
    QList<BusStop> busStopListComplete;
    QList<Bus> busListComplete;

    // supporting methods
    void initializeLists();
    void initializeWidgets();
    void initializeConnections();
    void initializeValues();
    void setMap();
    void scaleMap(float zoom);
    void drawStop(int index);
    void drawBus(int index);
    QImage determineBusPNG(QString busService);
    void resetSearch();
    void runSearch(QString query);
};

}

#endif // BUSTRACKWINDOW_H

// vim: set ts=4 sw=4 et:
