#ifndef PLOTWIDGETXZOOMSTATE_H
#define PLOTWIDGETXZOOMSTATE_H

#include "core/plotwidgetstate/plotwidgetstate.h"

class PlotWidgetXZoomState : public PlotWidgetState
{

public:
    enum class ZoomType { BoundedZoom,
                          UnboundedZoom};

    explicit PlotWidgetXZoomState(PlotWidget *parent = 0);

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
    ZoomType mZoomType;

    void clearState();
};

#endif // PLOTWIDGETXZOOMSTATE_H
