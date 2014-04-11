#ifndef BUSTRACKWINDOW_H
#define BUSTRACKWINDOW_H

#include <QMainWindow>
#include <QWheelEvent>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QPixmap>
#include <QDebug>
#include <QSlider>

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
private slots:
    void zoomSlide(int newZoom);

private:
    Ui::BusTrackWindow *ui;
    QGraphicsScene mapScene;
    QPixmap mapPixmap;
    float currentZoom;
    bool canPan;
    int slideValue;

    // supporting methods
    void setMap();
    void scaleMap(float zoom);
};

#endif // BUSTRACKWINDOW_H
