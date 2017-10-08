#include "markedwidget.h"
#include <QPainter>
//#include <QtGlobal>

MarkedWidget::MarkedWidget(QWidget *parent) : QWidget(parent),
    mMarkingType(MarkingType::Corner), mMarkingColor(Qt::black)
{

}

void MarkedWidget::setMarkingColor(const QColor &color)
{
    if (mMarkingColor != color) {
        mMarkingColor = color;
    }
}

void MarkedWidget::setMarkingType(MarkedWidget::MarkingType type)
{
    if (mMarkingType != type) {
        mMarkingType = type;
    }
}

void MarkedWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);
    switch (mMarkingType) {
        case MarkingType::Corner:
            painter.setBrush(mMarkingColor);
            {
            QPoint topLeft     = QPoint(          0,            0);
            QPoint topRight    = QPoint(width() - 1,            0);
            QPoint bottomLeft  = QPoint(          0, height() - 1);
            QPoint bottomRight = QPoint(width() - 1, height() - 1);

            const int side = 10;
            painter.drawRect(QRect(topLeft                          , topLeft    + QPoint(side, side)));
            painter.drawRect(QRect(topRight    + QPoint(-side,    0), topRight   + QPoint(   0, side)));
            painter.drawRect(QRect(bottomLeft  + QPoint(    0,-side), bottomLeft + QPoint(side,    0)));
            painter.drawRect(QRect(bottomRight + QPoint(-side,-side), bottomRight                    ));
            }
            break;
        case MarkingType::Frame:
            {
            QPen pen(mMarkingColor);
            pen.setWidth(8);
            painter.setPen(pen);

            painter.drawRect(0, 0, width(), height());
            }
            break;
        case MarkingType::None:
            break;
    }
}

