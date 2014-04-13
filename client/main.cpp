#include "bustrackwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QFile styleFile(":/resources/BustrackStylesheet.qss");
    styleFile.open(QFile::ReadOnly);
    QString style(styleFile.readAll());
    a.setStyleSheet(style);
    BusTrackWindow w;
    w.show();

    return a.exec();
}
