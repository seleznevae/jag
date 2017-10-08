#include "basic/utility.h"
#include <QDebug>
#include <math.h>

namespace utility {

void drawRoundedRectWithShadow(QPainter &painter, const QRectF & rect, qreal xRadius, qreal yRadius, double shadowOffset, Qt::SizeMode mode)
{
    if (shadowOffset > 0) {
        painter.save();
        QRectF shadowRect = rect;
        shadowRect.moveTopLeft(shadowRect.topLeft() + QPointF(shadowOffset, shadowOffset));
        QLinearGradient shadowGradient(QPoint(0,0), QPoint(1,0));
        shadowGradient.setColorAt(0, QColor(120, 120, 120, 175));
        shadowGradient.setColorAt(1, QColor(160, 160, 160, 175));
        shadowGradient.setCoordinateMode(QGradient::ObjectBoundingMode);
        painter.setPen(Qt::NoPen);
        QBrush shadowBrush(shadowGradient);
        painter.setBrush(shadowBrush);
        painter.drawRoundedRect(shadowRect, xRadius, yRadius, mode);
        painter.restore();
    }

    painter.drawRoundedRect(rect, xRadius, yRadius, mode);
}



}



