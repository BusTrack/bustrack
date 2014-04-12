#include "mapview.h"
#include "bustrackwindow.h"

MapView::MapView(QWidget *parent) :
    QGraphicsView(parent)
{
}

void MapView::mousePressEvent(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton){
        QGraphicsView::mousePressEvent(event);
        event->ignore();
    }
}

void MapView::wheelEvent(QWheelEvent *event)
{
    event->ignore();
}
