#include "plotwidgetyzoomstate.h"
#include "global/global_definitions.h"

PlotWidgetYZoomState::PlotWidgetYZoomState(PlotWidget *parent)
    :PlotWidgetState(parent), mXStartZoom(-1), mYStartZoom(-1),
    mXCurrentPosition(-1), mYCurrentPosition(-1),
    mIsButtonPressed(false), mAxisIndexToZoom(-1), mZoomType(ZoomType::BoundedZoom)
{
}

void PlotWidgetYZoomState::treatMousePressEvent(QMouseEvent *event)
{
    if (mPlotWidget->axisRectCount() != 0) {
        mXStartZoom = event->x();
        mYStartZoom = event->y();

        QApplication::setOverrideCursor(Qt::SizeVerCursor);


            double x_coord = event->x();
            double y_coord = event->y();

            mAxisIndexToZoom = -1;
            for (int axisRectIndex = 0; axisRectIndex < mPlotWidget->axisRectCount(); ++axisRectIndex) {
                QRect axisRec = mPlotWidget->axisRect(axisRectIndex)->rect();
                if (((x_coord >= axisRec.x()) && (x_coord <= axisRec.x()+ axisRec.width()))
                    && ((y_coord >= axisRec.y()) && (y_coord <= axisRec.y()+ axisRec.height()))) {
                        mAxisIndexToZoom = axisRectIndex;
    //                    xStartZoom = mPlotWidget->axisRect(0)->axis(QCPAxis::atBottom)->pixelToCoord(event->x());
    //                    yStartZoom = mPlotWidget->axisRect(axisRectIndex)->axis(QCPAxis::atLeft)->pixelToCoord(event->y());
                        break;
                }
            }
            if (mAxisIndexToZoom == -1) {
                clearState();
            }
    }

}

void PlotWidgetYZoomState::treatMouseReleaseEvent(QMouseEvent *event)
{
    QApplication::restoreOverrideCursor();

    if (mPlotWidget->axisRectCount() != 0) {
        if (mAxisIndexToZoom != -1) {
            auto newY =  event->y();
            QRect axisRec = mPlotWidget->axisRect(mAxisIndexToZoom)->rect();

            if (mZoomType == ZoomType::BoundedZoom) {
                if (newY < axisRec.y())
                    newY = axisRec.y();
                if (newY > axisRec.y() + axisRec.height())
                    newY = axisRec.y() + axisRec.height();
            }

            double yStartZoom = mPlotWidget->axisRect(mAxisIndexToZoom)->axis(QCPAxis::atLeft)->pixelToCoord(mYStartZoom);
            double yEndZoom = mPlotWidget->axisRect(mAxisIndexToZoom)->axis(QCPAxis::atLeft)->pixelToCoord(newY - 1);  // -1 is fundamental and essential

            if (yStartZoom == yEndZoom) {
                auto delta = mPlotWidget->axisRect(mAxisIndexToZoom)->axis(QCPAxis::atLeft)->range().size() / 50.;
                yStartZoom -= delta;
                yEndZoom += delta;
            }
//            emit yZoomRequest(std::min(yStartZoom, yEndZoom), std::max(yStartZoom, yEndZoom), mAxisIndexToZoom);

            double yNewBegin;
            double yNewEnd;
            std::tie(yNewBegin, yNewEnd) = global::recountRangeWithRespectToPolicy(
                        std::tuple<double, double>(std::min(yStartZoom, yEndZoom), std::max(yStartZoom, yEndZoom)),
                        global::gYZoomRangeEvaluationPolicy);
            emit yZoomRequest(yNewBegin, yNewEnd, mAxisIndexToZoom);

            mIsButtonPressed = false;
        }
    }

    clearState();

}

void PlotWidgetYZoomState::treatMouseMoveEvent(QMouseEvent *event)
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

        } else {
            mIsButtonPressed = false;
        }
        mPlotWidget->update();
    }

}

void PlotWidgetYZoomState::treatWheelEvent(QWheelEvent *event)
{
    if (mPlotWidget->axisRectCount() != 0) {
        if (mPlotWidget->mLayoutModel->isWheelZoomEnabled()) {
            double yCenterZoom;
            double x_coord = event->x();
            double y_coord = event->y();

            for (int axisRectIndex = 0; axisRectIndex < mPlotWidget->axisRectCount(); ++axisRectIndex) {
                QRect axisRec = mPlotWidget->axisRect(axisRectIndex)->rect();
                if (((x_coord >= axisRec.x()) && (x_coord <= axisRec.x()+ axisRec.width()))
                    && ((y_coord >= axisRec.y()) && (y_coord <= axisRec.y()+ axisRec.height()))) {
                        yCenterZoom = mPlotWidget->axisRect(axisRectIndex)->axis(QCPAxis::atLeft)->pixelToCoord(event->y());

                        double factor = pow(0.85, event->delta()/120);
                        QCPRange yRange = mPlotWidget->axisRect(axisRectIndex)->axis(QCPAxis::atLeft)->range();
                        double upperRange = yCenterZoom + (yRange.upper - yCenterZoom)*factor;
                        double lowerRange = yCenterZoom - (yCenterZoom  - yRange.lower)*factor;

                        emit yZoomRequest(lowerRange, upperRange, axisRectIndex);

                }
            }
        }
    }

}

void PlotWidgetYZoomState::treatPaintEvent(QPaintEvent *)
{
    if (mPlotWidget->axisRectCount() != 0) {
        if (mIsButtonPressed == false) {
            if (mPlotWidget->getIndexOfAxisRectByPoint(QPoint(mXCurrentPosition, mYCurrentPosition)) != -1) {
                QPainter painter(mPlotWidget);
                painter.setPen(QPen(mPlotWidget->mPlotStyle.mColorPref.mGraficalPrimitivesColor));
                painter.drawLine(0, mYCurrentPosition, mPlotWidget->width(), mYCurrentPosition);
            }
            return;
        }

        if (mYStartZoom >= 0){
            QPainter painter(mPlotWidget);
            painter.setPen(QPen(mPlotWidget->mPlotStyle.mColorPref.mGraficalPrimitivesColor));
            painter.drawLine(0, mYStartZoom, mPlotWidget->width(), mYStartZoom);
            painter.drawLine(0, mYCurrentPosition, mPlotWidget->width(), mYCurrentPosition);
            painter.drawLine(mXStartZoom, mYStartZoom, mXStartZoom, mYCurrentPosition);

//            painter.drawLine(mXStartZoom, mYStartZoom,
//                             mXStartZoom +3, mYStartZoom+((mYCurrentPosition > mYStartZoom)? 10 : -10));
//            painter.drawLine(mXStartZoom, mYStartZoom,
//                             mXStartZoom -3, mYStartZoom+((mYCurrentPosition > mYStartZoom)? 10 : -10));
//            painter.drawLine(mXStartZoom, mYCurrentPosition,
//                             mXStartZoom +3, mYCurrentPosition+((mYCurrentPosition > mYStartZoom)? -10 : 10));
//            painter.drawLine(mXStartZoom, mYCurrentPosition,
//                             mXStartZoom -3, mYCurrentPosition+((mYCurrentPosition > mYStartZoom)? -10 : 10));

            drawLineEnding(painter, QVector2D(mXStartZoom, mYStartZoom),
                           QVector2D(0, (mYStartZoom <  mYCurrentPosition) ? -1 : 1), QCPLineEnding::esSpikeArrow);
            drawLineEnding(painter, QVector2D(mXStartZoom, mYCurrentPosition),
                           QVector2D(0, (mYStartZoom <  mYCurrentPosition) ? 1 : -1), QCPLineEnding::esSpikeArrow);

            if (global::gIsShadowInZoomModesEnabled) {
                painter.save();
                painter.setPen(Qt::NoPen);
                painter.setBrush(global::gZoomShadowColor);
                painter.drawRect(QRect(0, 0, mPlotWidget->width(), std::min(mYStartZoom, mYCurrentPosition)));
                painter.drawRect(QRect(0, std::max(mYStartZoom, mYCurrentPosition), mPlotWidget->width(), mPlotWidget->height()));
                painter.restore();
            }
        }
    }

}

void PlotWidgetYZoomState::initializeState()
{
    PlotWidgetState::initializeState();
    clearState();
}

void PlotWidgetYZoomState::clearState()
{

    mXStartZoom = -1;
    mYStartZoom = -1;
    mXCurrentPosition = -1;
    mYCurrentPosition = -1;
    mIsButtonPressed = false;
    mAxisIndexToZoom = -1;
    mZoomType = ZoomType::BoundedZoom;
}
