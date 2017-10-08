#ifndef HTMLDRAWING_H
#define HTMLDRAWING_H

class QPainter;
class QRect;
class QTextDocument;
#include <QSize>

void drawHtmlText(QPainter *painter, const QRect &rect, int flags, QTextDocument &text);

QSize sizeHint(QPainter *painter, int flags, QTextDocument &text);

#endif // HTMLDRAWING_H

