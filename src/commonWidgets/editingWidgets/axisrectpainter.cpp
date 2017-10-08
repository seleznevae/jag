#include "axisrectpainter.h"

#include <QPainter>
#include <QDebug>

AxisRectPainter::AxisRectPainter(QWidget *parent) : QWidget(parent)
{

}

void AxisRectPainter::setRectPosition(const utility::Positions &positions)
{
    mPositions = positions;
    update();
}

void AxisRectPainter::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);
    int wid = width();
    int hei = height();
    QPainter painter(this);
//    painter.setBrush(Qt::white);
    painter.setBrush(QWidget::palette().color(QWidget::backgroundRole()));
    painter.setPen(Qt::NoPen);
    painter.drawRect(QRectF(0, 0, wid, hei));
    QPen pen(QWidget::palette().color(QPalette::Foreground));
    painter.setPen(pen);
//    painter.setPen(Qt::black);
    if (mPositions.testFlag(utility::posLeft))
        painter.drawLine(wid/10, hei/10, wid/10, 9*hei/10);
    if (mPositions.testFlag(utility::posTop))
        painter.drawLine(wid/10, hei/10, 9*wid/10, hei/10);
    if (mPositions.testFlag(utility::posRight))
        painter.drawLine(9*wid/10, hei/10, 9*wid/10, 9*hei/10);
    if (mPositions.testFlag(utility::posBottom))
        painter.drawLine(wid/10, 9*hei/10, 9*wid/10, 9*hei/10);



//    painter.setBrush(QWidget::palette().color(QWidget::backgroundRole()));
//    painter.setPen(Qt::NoPen);
//    painter.drawRect(QRectF(0, 0, wid, hei));
//    QPen pen(QWidget::palette().color(QPalette::Foreground));
}
