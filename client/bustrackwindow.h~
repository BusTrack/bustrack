#ifndef BUSTRACKWINDOW_H
#define BUSTRACKWINDOW_H

#include <QMainWindow>
#include <QWheelEvent>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QPixmap>
#include <QDebug>

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

private:
    Ui::BusTrackWindow *ui;

    QGraphicsScene mapScene;
    QPixmap mapPixmap;
    float zoom;

    // supporting methods
    void setMap();
    void scaleMap(float zoom);
};

#endif // BUSTRACKWINDOW_H
