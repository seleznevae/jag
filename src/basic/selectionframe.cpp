#include "selectionframe.h"
#include <QPaintEvent>
#include <QPainter>
#include <QDebug>

SelectionFrame::SelectionFrame(Shape shape, QWidget *parent) :
QRubberBand(shape, parent)
{
    border_color = QColor(0, 83, 235);
    background_color = QColor(251, 122, 255);
}

SelectionFrame::~SelectionFrame()
{
}

void SelectionFrame::paintEvent(QPaintEvent *event)
{
//    Q_UNUSED(event);

    QPainter painter;
    QPen pen = QPen(border_color);
    pen.setWidth(5);
    pen.setStyle(Qt::DashLine);

    QBrush brush = QBrush(background_color);

    painter.begin(this);
    painter.setPen(pen);
    painter.setOpacity(0.5);
    painter.setBrush(brush);
    painter.drawRect(event->rect());
    painter.end();
}
