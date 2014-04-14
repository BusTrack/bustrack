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
    void createCalendarWidget();
    void createTimeWidget();
    void calendarSelectionChanged(int i);
    void timeSelectionChanged(int i);

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
    QList<QPushButton*> calButtonList;
    QList<QPushButton*> clockButtonList;
    QList<QLabel*> calTickList;
    QList<QLabel*> clockTickList;
    QSignalMapper *signalMapper;

    // supporting methods
    void initializeLists();
    void initializeWidgets();
    void initializeConnections();
    void initializeValues();
    void setMap();
    void scaleMap(float zoom);
    void drawStop(QString index,float offsetx, float offsety, int numPeople);
    void drawBus(QString busService, float offsetx, float offsety, int numPeople);
    QImage determineBusPNG(QString busService);
    void searchStop(QString name);
    void searchBus(QString name);
};

}

#endif // BUSTRACKWINDOW_H

// vim: set ts=4 sw=4 et:
