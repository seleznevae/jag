#include "plotwidgetxyzoomstate.h"
#include "basic/selectionframe.h"
#include "global/global_definitions.h"

PlotWidgetXYZoomState::PlotWidgetXYZoomState(PlotWidget *parent )
    :PlotWidgetState(parent), mXStartZoom(-1), mYStartZoom(-1),
    mXCurrentPosition(-1), mYCurrentPosition(-1),
    mIsButtonPressed(false), mAxisIndexToZoom(-1), mZoomType(ZoomType::BoundedZoom),
    mRubberBand(NULL)
{

}

void PlotWidgetXYZoomState::treatMousePressEvent(QMouseEvent *event)
{

    if (mPlotWidget->axisRectCount() != 0) {
        mXStartZoom = event->x();
        mYStartZoom = event->y();

        QApplication::setOverrideCursor(Qt::CrossCursor);

            double x_coord = event->x();
            double y_coord = event->y();

            mAxisIndexToZoom = -1;
            for (int axisRectIndex = 0; axisRectIndex < mPlotWidget->axisRectCount(); ++axisRectIndex) {
                QRect axisRec = mPlotWidget->axisRect(axisRectIndex)->rect();
                if (((x_coord >= axisRec.x()) && (x_coord <= axisRec.x()+ axisRec.width()))
                    && ((y_coord >= axisRec.y()) && (y_coord <= axisRec.y()+ axisRec.height()))) {
                        mAxisIndexToZoom = axisRectIndex;
                        break;
                }
            }

            mRubberBandOrigin = event->pos();
            if (!mRubberBand)
                mRubberBand = new SelectionFrame(QRubberBand::Rectangle, mPlotWidget);
            mRubberBand->setGeometry(QRect(mRubberBandOrigin, QSize()));
            mRubberBand->show();

            if (mAxisIndexToZoom == -1) {
                clearState();
            }
    }
}

void PlotWidgetXYZoomState::treatMouseReleaseEvent(QMouseEvent *event)
{
    QApplication::restoreOverrideCursor();

    if (mPlotWidget->axisRectCount() != 0) {
        if (mRubberBand)
            mRubberBand->hide();

        if ( mAxisIndexToZoom != -1) {
            auto newY =  event->y();
            auto newX =  event->x();
            QRect axisRec = mPlotWidget->axisRect(mAxisIndexToZoom)->rect();

            if (mZoomType == ZoomType::BoundedZoom) {
                if (newY < axisRec.y())
                    newY = axisRec.y();
                if (newY > axisRec.y() + axisRec.height())
                    newY = axisRec.y() + axisRec.height();
                if (newX < axisRec.x())
                    newX = axisRec.x();
                if (newX > axisRec.x() + axisRec.width())
                    newX = axisRec.x() + axisRec.width();
            }

            double xStartZoom = mPlotWidget->axisRect(0)->axis(QCPAxis::atBottom)->pixelToCoord(mXStartZoom);
            double yStartZoom = mPlotWidget->axisRect(mAxisIndexToZoom)->axis(QCPAxis::atLeft)->pixelToCoord(mYStartZoom);
            double xEndZoom = mPlotWidget->axisRect(0)->axis(QCPAxis::atBottom)->pixelToCoord(newX);
            double yEndZoom = mPlotWidget->axisRect(mAxisIndexToZoom)->axis(QCPAxis::atLeft)->pixelToCoord(newY - 1);  // -1 is fundamental and essential

            if (xStartZoom == xEndZoom) {
                auto delta = mPlotWidget->axisRect(0)->axis(QCPAxis::atBottom)->range().size() / 50.;
                xStartZoom -= delta;
                xEndZoom += delta;
            }
            if (yStartZoom == yEndZoom) {
                auto delta = mPlotWidget->axisRect(mAxisIndexToZoom)->axis(QCPAxis::atLeft)->range().size() / 50.;
                yStartZoom -= delta;
                yEndZoom += delta;
            }
//            emit xyZoomRequest(std::min(xStartZoom, xEndZoom), std::max(xStartZoom, xEndZoom),
//                               std::min(yStartZoom, yEndZoom), std::max(yStartZoom, yEndZoom), mAxisIndexToZoom);

            double xNewBegin;
            double xNewEnd;
            std::tie(xNewBegin, xNewEnd) = global::recountRangeWithRespectToPolicy(
                        std::tuple<double, double>(std::min(xStartZoom, xEndZoom), std::max(xStartZoom, xEndZoom)),
                        global::gXZoomRangeEvaluationPolicy);
            double yNewBegin;
            double yNewEnd;
            std::tie(yNewBegin, yNewEnd) = global::recountRangeWithRespectToPolicy(
                        std::tuple<double, double>(std::min(yStartZoom, yEndZoom), std::max(yStartZoom, yEndZoom)),
                        global::gYZoomRangeEvaluationPolicy);
            emit xyZoomRequest(xNewBegin, xNewEnd,
                   yNewBegin, yNewEnd, mAxisIndexToZoom);


            mIsButtonPressed = false;
        }
    }

    clearState();
}

void PlotWidgetXYZoomState::treatMouseMoveEvent(QMouseEvent *event)
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

                if (mAxisIndexToZoom != -1) {
                    QRect axisRecY= mPlotWidget->axisRect(mAxisIndexToZoom)->rect();
                    if (mYCurrentPosition < axisRecY.y())
                        mYCurrentPosition = axisRecY.y();
                    if (mYCurrentPosition > axisRecY.y() + axisRecY.height())
                        mYCurrentPosition = axisRecY.y() + axisRecY.height();
                }
            }

            mRubberBand->setGeometry(QRect(mRubberBandOrigin, QPoint(mXCurrentPosition, mYCurrentPosition)).normalized());

        } else {
            mIsButtonPressed = false;
        }
        mPlotWidget->update();
    }
}

void PlotWidgetXYZoomState::treatWheelEvent(QWheelEvent *event)
{
    if (mPlotWidget->axisRectCount() != 0) {
        if (mPlotWidget->mLayoutModel->isWheelZoomEnabled()) {
            double xCenterZoom;
            double yCenterZoom;
            double x_coord = event->x();
            double y_coord = event->y();

            for (int axisRectIndex = 0; axisRectIndex < mPlotWidget->axisRectCount(); ++axisRectIndex) {
                QRect axisRec = mPlotWidget->axisRect(axisRectIndex)->rect();
                if (((x_coord >= axisRec.x()) && (x_coord <= axisRec.x()+ axisRec.width()))
                    && ((y_coord >= axisRec.y()) && (y_coord <= axisRec.y()+ axisRec.height()))) {
                        xCenterZoom = mPlotWidget->axisRect(0)->axis(QCPAxis::atBottom)->pixelToCoord(event->x());
                        yCenterZoom = mPlotWidget->axisRect(axisRectIndex)->axis(QCPAxis::atLeft)->pixelToCoord(event->y());

                        double factor = pow(0.85, event->delta()/120);

                        QCPRange xRange =	mPlotWidget->axisRect(axisRectIndex)->axis(QCPAxis::atBottom)->range();
                        QCPRange yRange =	mPlotWidget->axisRect(axisRectIndex)->axis(QCPAxis::atLeft)->range();
                        double upperXRange = xCenterZoom + (xRange.upper - xCenterZoom)*factor;
                        double lowerXRange = xCenterZoom - (xCenterZoom  - xRange.lower)*factor;
                        double upperYRange = yCenterZoom + (yRange.upper - yCenterZoom)*factor;
                        double lowerYRange = yCenterZoom - (yCenterZoom  - yRange.lower)*factor;

                        emit xyZoomRequest(lowerXRange, upperXRange,
                                           lowerYRange, upperYRange, axisRectIndex);

                        break;
                }
            }

        }
    }
}

void PlotWidgetXYZoomState::treatPaintEvent(QPaintEvent *)
{
    if (mPlotWidget->axisRectCount() != 0) {
        if (mXStartZoom >= 0){
                QPainter painter(mPlotWidget);

                if (global::gIsShadowInZoomModesEnabled) {
                    painter.save();
                    painter.setPen(Qt::NoPen);
                    painter.setBrush(global::gZoomShadowColor);
                    painter.drawRect(QRect(0, 0, std::min(mXStartZoom, mXCurrentPosition), mPlotWidget->height()));
                    painter.drawRect(QRect(std::max(mXStartZoom, mXCurrentPosition), 0, mPlotWidget->width(), mPlotWidget->height()));
                    painter.drawRect(QRect(std::min(mXStartZoom, mXCurrentPosition), 0, std::abs(mXStartZoom - mXCurrentPosition), std::min(mYStartZoom, mYCurrentPosition)));
                    painter.drawRect(QRect(std::min(mXStartZoom, mXCurrentPosition), std::max(mYStartZoom, mYCurrentPosition), std::abs(mXStartZoom - mXCurrentPosition), mPlotWidget->height()));
                    painter.restore();
                }
        }
    }
}



void PlotWidgetXYZoomState::initializeState()
{
    PlotWidgetState::initializeState();
    clearState();
}

void PlotWidgetXYZoomState::clearState()
{

    mXStartZoom = -1;
    mYStartZoom = -1;
    mXCurrentPosition = -1;
    mYCurrentPosition = -1;
    mIsButtonPressed = false;
    mAxisIndexToZoom = -1;
    mZoomType = ZoomType::BoundedZoom;
}
