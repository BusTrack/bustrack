#ifndef BUSTRACKWINDOW_H
#define BUSTRACKWINDOW_H

#include <QMainWindow>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QPixmap>

namespace Ui {
class BusTrackWindow;
}

class BusTrackWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit BusTrackWindow(QWidget *parent = 0);
    ~BusTrackWindow();

private:
    Ui::BusTrackWindow *ui;
    QGraphicsScene mapScene;
    QPixmap mapPixmap;
};

#endif // BUSTRACKWINDOW_H
