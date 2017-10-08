#ifndef ARROWEDTEXT_H
#define ARROWEDTEXT_H

#include <QObject>
#include <QPointF>
#include <QColor>
#include "core/qcustomplot.h"
#include "plotStyle/plotstyle.h"


/*!
 \brief Class representing text with arrow in the plot.

*/
class ArrowedText : public QObject
{
    Q_OBJECT
public:
    enum class ArrowBasis {Right, Bottom, Left, Top};

    explicit ArrowedText(QObject *parent = 0);

    QString text() const;
    QPointF textCenter() const;
    QPointF arrowFinish() const;
    ArrowBasis arrowBasisInRanges(double xRange, double yRange) const;
//    double lineWidthF() const;
    QPen pen() const;
    QColor backgroundColor() const;
    QCPLineEnding::EndingStyle arrowStyle() const { return mArrowedTextPrefs.mArrowEndingStyle; }


signals:
    void arrowedTextChanged();

public slots:
    void setText(const QString& text);
    void setTextCenter(const QPointF& textCenter);
    void setArrowFinish(const QPointF& arrowFinish);
//    void setLineWidthF(double width);
    void setPen(const QPen &pen);
    void setBackgroundColor(QColor color);
    void setArrowStyle(QCPLineEnding::EndingStyle arrowStyle);
    void setArrowedTextPrefs(const style::ArrowedTextPref &prefs);

private:
    QPointF mTextCenter;
    QPointF mArrowFinish;
    QString mText;
//    double mLineWidth;
//    QPen mLinePen;
//    QColor mBackgroundColor;
//    QCPLineEnding::EndingStyle mArrowStyle;
    style::ArrowedTextPref mArrowedTextPrefs;
};

#endif // ARROWEDTEXT_H
