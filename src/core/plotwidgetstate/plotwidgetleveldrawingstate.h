#ifndef PLOTWIDGETLEVELDRAWINGSTATE_H
#define PLOTWIDGETLEVELDRAWINGSTATE_H

#include "core/plotwidgetstate/plotwidgetstate.h"

class PlotWidgetLevelDrawingState : public PlotWidgetState
{
public:
    PlotWidgetLevelDrawingState();

    explicit PlotWidgetLevelDrawingState(PlotWidget *par = NULL);

    virtual void treatMousePressEvent   (QMouseEvent  *);
    virtual void treatMouseMoveEvent    (QMouseEvent  *);
    virtual void treatPaintEvent        (QPaintEvent  *);
    virtual void treatLeaveEvent        (QEvent       *);

    virtual void initializeState();
    virtual void setCursorShape();

private:
    int mXCurrentPosition; //xCurrentZoomWidget
    int mYCurrentPosition; //yCurrentZoomWidget


    void drawTracers(QPainter&);
};

#endif // PLOTWIDGETLEVELDRAWINGSTATE_H


