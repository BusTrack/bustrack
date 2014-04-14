#include <QApplication>
#include "bustrackwindow.h"

int main(int argc, char *argv[])
{
    // Prepare application stylesheet.
    QFile styleFile(":/resources/BustrackStylesheet.qss");
    styleFile.open(QFile::ReadOnly);
    QString style(styleFile.readAll());

    // Create the application and set the style sheet.
    QApplication app(argc, argv);
    app.setStyleSheet(style);

    // Create the main BusTrack window.
    bustrack::BusTrackWindow window;
    window.show();

    // Run the app.
    return app.exec();
}

// vim: set ts=4 sw=4 et:
