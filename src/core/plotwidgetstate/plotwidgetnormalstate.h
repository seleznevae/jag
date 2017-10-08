#ifndef PLOTWIDGETNORMALSTATE_H
#define PLOTWIDGETNORMALSTATE_H


#include "core/plotwidgetstate/plotwidgetstate.h"



class PlotWidgetNormalState : public PlotWidgetState
{
public:
    enum class State {Off,
                      On
                     };

    explicit PlotWidgetNormalState(PlotWidget *parent = 0);


    virtual void treatMousePressEvent   (QMouseEvent  *);
    virtual void treatMouseReleaseEvent (QMouseEvent  *);
    virtual void treatMouseMoveEvent    (QMouseEvent  *);
//    virtual void treatWheelEvent        (QWheelEvent  *) { }
    virtual void treatPaintEvent        (QPaintEvent  *);
//    virtual void treatResizeEvent       (QResizeEvent *) { }
    virtual void treatLeaveEvent        (QEvent       *);
    virtual void treatDropEvent         (QDropEvent   *);
    virtual void treatDragMoveEvent     (QDragMoveEvent *);
    virtual void treatDragLeaveEvent    (QDragLeaveEvent*);
    virtual void treatDragEnterEvent    (QDragEnterEvent*);

//    virtual void initializeState();
//    virtual void setInteractions();

private:


    QPoint mStartDragPos;
    QPoint mTextStartDragPos;
    QPoint mArrowEndStartDragPos;
    int mDraggedGraphIndex;
    int mDraggedArrowedTextIndex;
    int mDraggedArrowEndIndex;
    State mGraphDragState;
    State mArrowedTextDragState;
    State mArrowEndDragState;
    QPoint mCurrentPos;
private:
    void highlightAxisRectUnderPosition(QPoint currentPos);

};

class GraphMimeData: public QMimeData {
    Q_OBJECT
public:
    GraphModel *mDraggedGraph = nullptr;

};

#endif // PLOTWIDGETNORMALSTATE_H
