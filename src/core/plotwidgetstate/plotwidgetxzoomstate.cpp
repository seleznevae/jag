#include "plotwidgetxzoomstate.h"
#include "core/qcustomplot.h"
#include "global/global_definitions.h"

PlotWidgetXZoomState::PlotWidgetXZoomState(PlotWidget *parent) :
    PlotWidgetState(parent), mXStartZoom(-1), mYStartZoom(-1),
    mXCurrentPosition(-1), mYCurrentPosition(-1),
    mIsButtonPressed(false), mZoomType(ZoomType::BoundedZoom)
{


}

void PlotWidgetXZoomState::treatMousePressEvent(QMouseEvent *event)
{
    if (mPlotWidget->axisRectCount() != 0) {
        mXStartZoom = event->x();
        mYStartZoom = event->y();

        QApplication::setOverrideCursor(Qt::SizeHorCursor);
    }
}

void PlotWidgetXZoomState::treatMouseReleaseEvent(QMouseEvent *event)
{
    QApplication::restoreOverrideCursor();


    if (mPlotWidget->axisRectCount() != 0) {
        auto newX =  event->x();
        QRect axisRec = mPlotWidget->axisRect(0)->rect();

        if (mZoomType == ZoomType::BoundedZoom) {
            if (newX < axisRec.x())
                newX = axisRec.x();
            if (newX > axisRec.x() + axisRec.width())
                newX = axisRec.x() + axisRec.width();
        }

        double xStartZoom = mPlotWidget->axisRect(0)->axis(QCPAxis::atBottom)->pixelToCoord(mXStartZoom);
        double xEndZoom = mPlotWidget->axisRect(0)->axis(QCPAxis::atBottom)->pixelToCoord(newX);

        if (xStartZoom == xEndZoom) {
            auto delta = mPlotWidget->axisRect(0)->axis(QCPAxis::atBottom)->range().size() / 50.;
            xStartZoom -= delta;
            xEndZoom += delta;
        }
//        emit xZoomRequest(std::min(xStartZoom, xEndZoom), std::max(xStartZoom, xEndZoom));

        double xNewBegin;
        double xNewEnd;
        std::tie(xNewBegin, xNewEnd) = global::recountRangeWithRespectToPolicy(
                    std::tuple<double, double>(std::min(xStartZoom, xEndZoom), std::max(xStartZoom, xEndZoom)),
                    global::gXZoomRangeEvaluationPolicy);
        emit xZoomRequest(xNewBegin, xNewEnd);


        mIsButtonPressed = false;
    }
    clearState();

}

void PlotWidgetXZoomState::treatMouseMoveEvent(QMouseEvent *event)
{
    if (mPlotWidget->axisRectCount() != 0) {
        mXCurrentPosition = event->x();
        mYCurrentPosition = event->y();

        Qt::MouseButtons buttons = event->buttons();
        if ((buttons &  Qt::LeftButton) || (buttons &  Qt::RightButton)) {
            if (buttons &  Qt::LeftButton)
                mZoomType = ZoomType::BoundedZoom;
            else if (buttons &  Qt::RightButton)
                mZoomType = ZoomType::UnboundedZoom;

            mIsButtonPressed = true;

            mXCurrentPosition = event->x();
            mYCurrentPosition = event->y();

            if (mZoomType == ZoomType::BoundedZoom) {
                QRect axisRec = mPlotWidget->axisRect(0)->rect();
                if (mXCurrentPosition < axisRec.x())
                    mXCurrentPosition = axisRec.x();
                if (mXCurrentPosition > axisRec.x() + axisRec.width())
                        mXCurrentPosition = axisRec.x() + axisRec.width();
            }

        } else {
            mIsButtonPressed = false;
        }
        mPlotWidget->update();
    }
}

void PlotWidgetXZoomState::treatWheelEvent(QWheelEvent *event)
{
    if (mPlotWidget->axisRectCount() != 0) {
        if (mPlotWidget->mLayoutModel->isWheelZoomEnabled()) {

            double xCenterZoom;
            double x_coord = event->x();
            double y_coord = event->y();

            for (int axisRectIndex = 0; axisRectIndex < mPlotWidget->axisRectCount(); ++axisRectIndex) {
                QRect axisRec = mPlotWidget->axisRect(axisRectIndex)->rect();
                if (((x_coord >= axisRec.x()) && (x_coord <= axisRec.x()+ axisRec.width()))
                    && ((y_coord >= axisRec.y()) && (y_coord <= axisRec.y()+ axisRec.height()))) {
                        xCenterZoom = mPlotWidget->axisRect(0)->axis(QCPAxis::atBottom)->pixelToCoord(event->x());

                        double factor = pow(0.85, event->delta()/120);
                        QCPRange xRange =	mPlotWidget->axisRect(axisRectIndex)->axis(QCPAxis::atBottom)->range();
                        double upperRange = xCenterZoom + (xRange.upper - xCenterZoom)*factor;
                        double lowerRange = xCenterZoom - (xCenterZoom  - xRange.lower)*factor;
                        emit xZoomRequest(lowerRange, upperRange);

                        break;
                }
            }

        }
    }

}

void PlotWidgetXZoomState::treatPaintEvent(QPaintEvent */*event*/)
{
    if (mPlotWidget->axisRectCount() != 0) {
        if (mIsButtonPressed == false) {
            if (mPlotWidget->getIndexOfAxisRectByPoint(QPoint(mXCurrentPosition, mYCurrentPosition)) != -1) {
                QPainter painter(mPlotWidget);
                painter.setPen(QPen(mPlotWidget->mPlotStyle.mColorPref.mGraficalPrimitivesColor/*global::gDefaultPlotGraphicalPrimitivesColor*/));
                painter.drawLine(mXCurrentPosition, 0, mXCurrentPosition, mPlotWidget->height());
            }
            return;
        }

        if (mXStartZoom >= 0){
                QPainter painter(mPlotWidget);
                painter.setPen(QPen(mPlotWidget->mPlotStyle.mColorPref.mGraficalPrimitivesColor));

                painter.drawLine(mXStartZoom, 0, mXStartZoom, mPlotWidget->height());
                painter.drawLine(mXCurrentPosition, 0, mXCurrentPosition, mPlotWidget->height());
                painter.drawLine(mXStartZoom, mYStartZoom, mXCurrentPosition, mYStartZoom);

//                painter.drawLine(mXStartZoom, mYStartZoom,
//                                 mXStartZoom + ((mXCurrentPosition > mXStartZoom)? 10 :-10), mYStartZoom+3);
//                painter.drawLine(mXStartZoom, mYStartZoom,
//                                 mXStartZoom + ((mXCurrentPosition > mXStartZoom)? 10 :-10), mYStartZoom-3);
//                painter.drawLine(mXCurrentPosition, mYStartZoom,
//                                 mXCurrentPosition + ((mXCurrentPosition > mXStartZoom)? -10 :10), mYStartZoom+3);
//                painter.drawLine(mXCurrentPosition, mYStartZoom,
//                                 mXCurrentPosition + ((mXCurrentPosition > mXStartZoom)? -10 :10), mYStartZoom-3);

                drawLineEnding(painter, QVector2D(mXStartZoom, mYStartZoom),
                               QVector2D((mXStartZoom <  mXCurrentPosition) ? -1 : 1, 0), QCPLineEnding::esSpikeArrow);
                drawLineEnding(painter, QVector2D(mXCurrentPosition, mYStartZoom),
                               QVector2D((mXStartZoom <  mXCurrentPosition) ? 1 : -1, 0), QCPLineEnding::esSpikeArrow);

                if (global::gIsShadowInZoomModesEnabled) {
                    painter.save();
                    painter.setPen(Qt::NoPen);
                    painter.setBrush(global::gZoomShadowColor);
                    painter.drawRect(QRect(0, 0, std::min(mXStartZoom, mXCurrentPosition), mPlotWidget->height()));
                    painter.drawRect(QRect(std::max(mXStartZoom, mXCurrentPosition), 0, mPlotWidget->width(), mPlotWidget->height()));
                    painter.restore();
                }
        }
    }
}

void PlotWidgetXZoomState::initializeState()
{
    PlotWidgetState::initializeState();
    clearState();
}

void PlotWidgetXZoomState::clearState()
{
    mXStartZoom = -1;
    mYStartZoom = -1;
    mXCurrentPosition = -1;
    mYCurrentPosition = -1;
    mIsButtonPressed = false;
    mZoomType = ZoomType::BoundedZoom;
}

