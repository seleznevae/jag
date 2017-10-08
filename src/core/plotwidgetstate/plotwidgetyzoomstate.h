#ifndef PLOTWIDGETYZOOMSTATE_H
#define PLOTWIDGETYZOOMSTATE_H

#include "core/plotwidgetstate/plotwidgetstate.h"

class PlotWidgetYZoomState : public PlotWidgetState
{
public:
    enum class ZoomType { BoundedZoom,
                          UnboundedZoom};

    explicit PlotWidgetYZoomState(PlotWidget *parent = 0);

    virtual void treatMousePressEvent   (QMouseEvent  *);
    virtual void treatMouseReleaseEvent (QMouseEvent  *);
    virtual void treatMouseMoveEvent    (QMouseEvent  *);
    virtual void treatWheelEvent        (QWheelEvent  *);
    virtual void treatPaintEvent        (QPaintEvent  *);

    virtual void initializeState();
signals:

public slots:

private:
    int mXStartZoom;
    int mYStartZoom;
    int mXCurrentPosition;
    int mYCurrentPosition;
    bool mIsButtonPressed;
    int mAxisIndexToZoom;
    ZoomType mZoomType;

    void clearState();
};

#endif // PLOTWIDGETYZOOMSTATE_H
