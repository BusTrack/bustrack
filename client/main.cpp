#include "bustrackwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    BusTrackWindow w;
    w.show();

    return a.exec();
}
