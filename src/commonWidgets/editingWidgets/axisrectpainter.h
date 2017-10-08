#ifndef AXISRECTPAINTER_H
#define AXISRECTPAINTER_H

#include <QWidget>
#include "plotStyle/plotstyle.h"

class AxisRectPainter : public QWidget
{
    Q_OBJECT
public:
    explicit AxisRectPainter(QWidget *parent = 0);

    void setRectPosition(const utility::Positions &positions);
signals:

protected:
    void paintEvent(QPaintEvent *);
public slots:

private:
    utility::Positions mPositions;
};

#endif // AXISRECTPAINTER_H
