#ifndef PLOTWIDGETARROWDRAWINGSTATE_H
#define PLOTWIDGETARROWDRAWINGSTATE_H


#include "core/plotwidgetstate/plotwidgetstate.h"

class PlotWidgetArrowDrawingState: public PlotWidgetState
{
    enum class ArrowType {Internal,
                          External
                         };
public:
    explicit PlotWidgetArrowDrawingState(PlotWidget *par = NULL);

    virtual void treatMousePressEvent   (QMouseEvent  *);
    virtual void treatMouseReleaseEvent (QMouseEvent  *);
    virtual void treatMouseMoveEvent    (QMouseEvent  *);
    virtual void treatPaintEvent        (QPaintEvent  *);

    virtual void initializeState();

private:
    int mXStart;
    int mYStart;
    int mXCurrentPosition;
    int mYCurrentPosition;
    bool mIsButtonPressed;
    int mAxisIndex;
    ArrowType mArrowType;

    void clearState();

};

#endif // PLOTWIDGETARROWDRAWINGSTATE_H
