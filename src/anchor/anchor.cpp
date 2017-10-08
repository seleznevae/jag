#include "anchor.h"
#include "global/global_definitions.h"


Anchor::Anchor(double time, QObject *parent)
    :QObject(parent), mTime(time), mName(QString::number(time)), mStyle(QCPItemTracer::tsCircle),
    mSize(global::gTracerSize), mBrushColor(Qt::white), mBorderPen(Qt::red)
{

}

void Anchor::setTime(double time)
{
    if (mTime != time) {
        mTime = time;
        emit anchorChanged();
    }
}

void Anchor::setName(const QString &name)
{
    if (mName != name) {
        mName = name;
        emit anchorChanged();
    }
}

void Anchor::setStyle(QCPItemTracer::TracerStyle style)
{
    if (mStyle != style) {
        mStyle = style;
        emit anchorChanged();
    }
}

void Anchor::setSize(double size)
{
    if (mSize != size) {
        mSize = size;
        emit anchorChanged();
    }
}

void Anchor::setBrushColor(QColor color)
{
    if (mBrushColor != color) {
        mBrushColor = color;
        emit anchorChanged();
    }

}

void Anchor::setBorderPen(const QPen &pen)
{
    if (mBorderPen != pen) {
        mBorderPen = pen;
        emit anchorChanged();
    }

}

void Anchor::setBorderPenColor(const QColor &color)
{
    if (mBorderPen.color() != color) {
        mBorderPen.setColor(color);
        emit anchorChanged();
    }

}

void Anchor::setBorderPenWidth(double width)
{
    if (mBorderPen.widthF() != width) {
        mBorderPen.setWidthF(width);
        emit anchorChanged();
    }

}


