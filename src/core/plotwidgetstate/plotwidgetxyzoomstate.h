#ifndef PLOTWIDGETXYZOOMSTATE_H
#define PLOTWIDGETXYZOOMSTATE_H

#include "core/plotwidgetstate/plotwidgetstate.h"

class SelectionFrame;

class PlotWidgetXYZoomState : public PlotWidgetState
{
public:
    enum class ZoomType { BoundedZoom,
                          UnboundedZoom};

    explicit PlotWidgetXYZoomState(PlotWidget *parent = 0);


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

    SelectionFrame *mRubberBand; //could be RubberBand as well
    QPoint mRubberBandOrigin;

    void clearState();
};

#endif // PLOTWIDGETXYZOOMSTATE_H
