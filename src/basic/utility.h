#ifndef UTILITY
#define UTILITY

#include <QPainter>

namespace utility {

void drawRoundedRectWithShadow(QPainter &painter, const QRectF & rect, qreal xRadius, qreal yRadius, double shadowOffset, Qt::SizeMode mode = Qt::AbsoluteSize);

}
#endif // UTILITY

