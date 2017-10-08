#ifndef PLOTWIDGETFASTSECONDARYPROCESSINGSTATE_H
#define PLOTWIDGETFASTSECONDARYPROCESSINGSTATE_H

#include "core/plotwidgetstate/plotwidgetstate.h"

class PlotWidgetFastSecondaryProcessingState : public PlotWidgetState
{
public:
    enum class ZoomType { BoundedZoom,
                          UnboundedZoom};

    explicit PlotWidgetFastSecondaryProcessingState(PlotWidget *parent = 0);

    virtual void treatMousePressEvent   (QMouseEvent  *);
    virtual void treatMouseReleaseEvent (QMouseEvent  *);
    virtual void treatMouseMoveEvent    (QMouseEvent  *);
    virtual void treatPaintEvent        (QPaintEvent  *);

    virtual void initializeState();
signals:

private:
    int mXStartZoom;
    int mYStartZoom;
    int mXCurrentPosition;
    int mYCurrentPosition;
    bool mIsButtonPressed;
    int mAxisIndexToZoom;
    ZoomType mZoomType;

//    SelectionFrame *mRubberBand; //could be RubberBand as well
//    QPoint mRubberBandOrigin;

    void clearState();
};


#endif // PLOTWIDGETFASTSECONDARYPROCESSINGSTATE_H


