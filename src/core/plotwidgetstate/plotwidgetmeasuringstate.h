#ifndef PLOTWIDGETMEASURINGSTATE_H
#define PLOTWIDGETMEASURINGSTATE_H

#include "core/plotwidgetstate/plotwidgetstate.h"

class PlotWidgetMeasuringState : public PlotWidgetState
{
public:
    explicit PlotWidgetMeasuringState(PlotWidget *par = NULL);

    virtual void treatMousePressEvent   (QMouseEvent  *);
    virtual void treatMouseMoveEvent    (QMouseEvent  *);
    virtual void treatMouseReleaseEvent (QMouseEvent  *);
    virtual void treatPaintEvent        (QPaintEvent  *);
    virtual void treatLeaveEvent        (QEvent       *);

    virtual void initializeState();
    virtual void setCursorShape();

private:
    int mXCurrentPosition; //xCurrentZoomWidget
    int mYCurrentPosition; //yCurrentZoomWidget
    int mXStartPosition;
    int mYStartPosition;
    bool mIsButtonPressed;
    int mAxisIndexToMeasure;

    void clearState();

    void drawTracers(QPainter&);


};

#endif // PLOTWIDGETMEASURINGSTATE_H
