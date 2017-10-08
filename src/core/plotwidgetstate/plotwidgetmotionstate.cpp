#include "plotwidgetmotionstate.h"

PlotWidgetMotionState::PlotWidgetMotionState(PlotWidget *parent) :
    PlotWidgetState(parent), mMovingAxisNumber(-1)
{
}

void PlotWidgetMotionState::treatMousePressEvent(QMouseEvent *event)
{
    if (mPlotWidget->axisRectCount() != 0) {
        //cursor appearance changed
        QApplication::setOverrideCursor(Qt::OpenHandCursor);
        mMovingAxisNumber = mPlotWidget->getIndexOfAxisRectByPoint(event->pos());
    }
}

void PlotWidgetMotionState::treatMouseReleaseEvent(QMouseEvent */*event*/)
{
    QApplication::restoreOverrideCursor();

    if (mPlotWidget->axisRectCount() != 0) {
        if (mMovingAxisNumber >=0 && mMovingAxisNumber < mPlotWidget->axisRectCount()) {
            QCPRange xRange = mPlotWidget->axisRect(mMovingAxisNumber)->axis(QCPAxis::atBottom)->range();
            QCPRange yRange = mPlotWidget->axisRect(mMovingAxisNumber)->axis(QCPAxis::atLeft)->range();
            emit xyZoomRequest(xRange.lower, xRange.upper,
                               yRange.lower, yRange.upper, mMovingAxisNumber);
            mMovingAxisNumber = -1;
        }
    }

}

void PlotWidgetMotionState::treatMouseMoveEvent(QMouseEvent *event)
{
    if (mPlotWidget->axisRectCount() != 0) {
        if (event->modifiers() == Qt::ControlModifier) {
            for (int i = 0; i < mPlotWidget->axisRectCount(); ++i) {
                mPlotWidget->axisRect(i)->setRangeDrag(Qt::Horizontal);
            }
        } else if (event->modifiers() == (Qt::ControlModifier | Qt::ShiftModifier)) {
            for (int i = 0; i < mPlotWidget->axisRectCount(); ++i) {
                mPlotWidget->axisRect(i)->setRangeDrag(Qt::Vertical);
            }
        } else {
            for (int i = 0; i < mPlotWidget->axisRectCount(); ++i) {
                mPlotWidget->axisRect(i)->setRangeDrag(Qt::Horizontal | Qt::Vertical);
            }
        }
    }

}


void PlotWidgetMotionState::setInteractions()
{
    mPlotWidget->setInteractions(QCP::iRangeDrag);
}
