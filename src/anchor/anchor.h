#ifndef ANCHOR_H
#define ANCHOR_H

#include <QString>
#include <QObject>
#include "core/qcustomplot.h"

class Anchor: public QObject
{
    Q_OBJECT
public:
    Anchor(double time, QObject* parent = NULL);

    double time() const {return mTime;}
    QString name() const {return mName;}
    QCPItemTracer::TracerStyle style() const { return mStyle; }
    double size() const { return mSize; }
    QColor brushColor() const { return mBrushColor; }
    QPen borderPen() const { return mBorderPen; }

    void setTime(double time);
    void setName(const QString &name);
    void setStyle(QCPItemTracer::TracerStyle style);
    void setSize(double size);
    void setBrushColor(QColor);
    void setBorderPen(const QPen&);
    void setBorderPenColor(const QColor&);
    void setBorderPenWidth(double);

signals:
    void anchorChanged();


private:
    double mTime;
    QString mName;
    QCPItemTracer::TracerStyle mStyle;
    double mSize;
    QColor mBrushColor;
    QPen mBorderPen;

};

#endif // ANCHOR_H
