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

namespace Ui {
class BusTrackWindow;
}

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

private slots:
    void zoomSlide(int newZoom);
    void toggleSearchResultsWidget(QString query);
    void onBusInfoBtnClicked();
    void onStopInfoBtnClicked();
    void toggleElementsVisibility();
    void toggleDispatchWidget();
    void createCalendarWidget();
    void createTimeWidget();

private:
    Ui::BusTrackWindow *ui;
    QGraphicsScene mapScene;
    QGraphicsPixmapItem* mapPixmap;
    QPixmap map;
    QMenu *fileMenu;
    QAction *hideAction;
    QAction *dispatchAction;

    float currentZoom;
    int slideValue;
    bool busInfoBtnClicked;
    bool stopInfoBtnClicked;
    bool hidden;
    bool dispatchWidgetVisible;
    QHash<QString,QGraphicsItem*> busstopHash;
    bool searchActive;
    QGraphicsPathItem* searchOverlay;

    // supporting methods
    void initializeWidgets();
    void initializeConnections();
    void initializeValues();
    void setMap();
    void scaleMap(float zoom);
    void drawStop(QString name,int offsetx, int offsety, int numPeople);
    void drawBus(QString busService, float offsetx, float offsety, int numPeople);
    QImage determineBusPNG(QString busService);
    void searchStop(QString name);
};

#endif // BUSTRACKWINDOW_H
