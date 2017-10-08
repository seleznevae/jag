#ifndef PLOTWIDGETMOTIONSTATE_H
#define PLOTWIDGETMOTIONSTATE_H

#include "core/plotwidgetstate/plotwidgetstate.h"

class PlotWidgetMotionState : public PlotWidgetState
{
public:
    explicit PlotWidgetMotionState(PlotWidget *parent = 0);


    virtual void treatMousePressEvent   (QMouseEvent  *);
    virtual void treatMouseReleaseEvent (QMouseEvent  *);
    virtual void treatMouseMoveEvent    (QMouseEvent  *);
//    virtual void treatWheelEvent        (QWheelEvent  *) { }
//    virtual void treatPaintEvent        (QPaintEvent  *) { }
//    virtual void treatResizeEvent       (QResizeEvent *) { }
//    virtual void treatLeaveEvent        (QEvent       *) { }

    virtual void setInteractions();
signals:

public slots:
    int mMovingAxisNumber;

};

#endif // PLOTWIDGETMOTIONSTATE_H
