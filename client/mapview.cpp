#include "mapview.h"
#include "bustrackwindow.h"

MapView::MapView(QWidget *parent) :
    QGraphicsView(parent)
{
	setMouseTracking(true);
}

void MapView::mousePressEvent(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton || event->button() == Qt::RightButton){
        QGraphicsView::mousePressEvent(event);
        event->ignore();
    }
}

void MapView::wheelEvent(QWheelEvent *event)
{
    event->ignore();
}

/*
void MapView::mouseMoveEvent(QMouseEvent* event)
{
   qDebug() << "You hovered";
}
*/
