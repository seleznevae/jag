#include "basic/htmldrawing.h"

#include <QPainter>
#include <QTextDocument>
#include <QAbstractTextDocumentLayout>
#include "core/qcustomplot.h"

void drawHtmlText(QPainter *painter, const QRect &rect, int flags, QTextDocument &text)
{

    sizeHint(painter, flags, text);

    text.setPageSize(QSize(rect.width(), QWIDGETSIZE_MAX));
//    text.setPageSize(QSize(10000, QWIDGETSIZE_MAX));

    text.setDefaultFont(painter->font());

    QAbstractTextDocumentLayout* layout = text.documentLayout();

    const int height = qRound(layout->documentSize().height());
    int y = rect.y();
    if (flags & Qt::AlignBottom)
        y += (rect.height() - height);
    else if (flags & Qt::AlignVCenter)
        y += (rect.height() - height)/2;

    if (flags & Qt::AlignHCenter) {
        text.setHtml("<p align=center>" + text.toHtml() + "</p>");
    }

    if (flags & Qt::AlignLeft) {
        text.setHtml("<p align=left>" + text.toHtml() + "</p>");
    }

    text.defaultTextOption().setWrapMode(QTextOption::NoWrap);

//    text.setIndentWidth(0);
//    text.setDocumentMargin(0);
//    text.setDefaultTextOption(text.defaultTextOption().setWrapMode(QTextOption::NoWrap));

    QAbstractTextDocumentLayout::PaintContext context;
    context.palette.setColor(QPalette::Text, painter->pen().color());

    painter->save();

    painter->translate(rect.x(), y);
    layout->draw(painter, context);

    painter->restore();

}


QSize sizeHint(QPainter *painter, int flags, QTextDocument &textArg)
{
    QTextDocument *textP = textArg.clone();
    QTextDocument &text = *textP;

    text.defaultTextOption().setWrapMode(QTextOption::NoWrap);
//    text.setDefaultTextOption(text.defaultTextOption().setWrapMode(QTextOption::NoWrap));

    text.setPageSize(QSize(10000, 10000));
    text.setDefaultFont(painter->font());

//    QAbstractTextDocumentLayout* layout = text.documentLayout();

//    const int height = qRound(layout->documentSize().height());

    if (flags & Qt::AlignHCenter) {
        text.setHtml("<p align=center>" + text.toHtml() + "</p>");
    }

    if (flags & Qt::AlignLeft) {
        text.setHtml("<p align=left>" + text.toHtml() + "</p>");
    }

//    text.setIndentWidth(0);
//    text.setDocumentMargin(0);


    QAbstractTextDocumentLayout::PaintContext context;
    context.palette.setColor(QPalette::Text, painter->pen().color());

    return QSize(text.idealWidth(), /*text.size().height()*/ QFontMetrics(painter->font()).boundingRect(text.toHtml()).height());
}
