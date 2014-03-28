#ifndef BUSTRACKWINDOW_H
#define BUSTRACKWINDOW_H

#include <QMainWindow>

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
};

#endif // BUSTRACKWINDOW_H
