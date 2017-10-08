#include "plotwidgetfastsecondaryprocessingstate.h"
#include "global/global_definitions.h"

PlotWidgetFastSecondaryProcessingState::PlotWidgetFastSecondaryProcessingState(PlotWidget *parent )
    :PlotWidgetState(parent), mXStartZoom(-1), mYStartZoom(-1),
    mXCurrentPosition(-1), mYCurrentPosition(-1),
    mIsButtonPressed(false), mAxisIndexToZoom(-1), mZoomType(ZoomType::BoundedZoom)
{

}


void PlotWidgetFastSecondaryProcessingState::treatMousePressEvent(QMouseEvent *event)
{
    if (mPlotWidget->axisRectCount() != 0) {
        mXStartZoom = event->x();
        mYStartZoom = event->y();

        QApplication::setOverrideCursor(Qt::SizeHorCursor);

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

//            mRubberBandOrigin = event->pos();
//            if (!mRubberBand)
//                mRubberBand = new SelectionFrame(QRubberBand::Rectangle, mPlotWidget);
//            mRubberBand->setGeometry(QRect(mRubberBandOrigin, QSize()));
//            mRubberBand->show();

            if (mAxisIndexToZoom == -1) {
                clearState();
            }
    }
}

void PlotWidgetFastSecondaryProcessingState::treatMouseReleaseEvent(QMouseEvent *event)
{
    QApplication::restoreOverrideCursor();
    if (mPlotWidget->axisRectCount() != 0) {

        if ( mAxisIndexToZoom != -1) {
            auto newX =  event->x();
            QRect axisRec = mPlotWidget->axisRect(mAxisIndexToZoom)->rect();

            if (mZoomType == ZoomType::BoundedZoom) {
                if (newX < axisRec.x())
                    newX = axisRec.x();
                if (newX > axisRec.x() + axisRec.width())
                    newX = axisRec.x() + axisRec.width();
            }

            double xStart = mPlotWidget->axisRect(0)->axis(QCPAxis::atBottom)->pixelToCoord(mXStartZoom);
            double xFinish = mPlotWidget->axisRect(0)->axis(QCPAxis::atBottom)->pixelToCoord(newX);

            emit mPlotWidget->fastSecondaryProcessingRequest(std::min(xStart, xFinish), std::max(xStart, xFinish),
                                                             mPlotWidget->getIndexOfAxisRectByPoint(QPoint(mXStartZoom, mYStartZoom)));

            mIsButtonPressed = false;
        }
    }
    clearState();
}

void PlotWidgetFastSecondaryProcessingState::treatMouseMoveEvent(QMouseEvent *event)
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

//            mRubberBand->setGeometry(QRect(mRubberBandOrigin, QPoint(mXCurrentPosition, mYCurrentPosition)).normalized());

        } else {
            mIsButtonPressed = false;
        }
        mPlotWidget->update();
    }
}

void PlotWidgetFastSecondaryProcessingState::treatPaintEvent(QPaintEvent *)
{
    if (mPlotWidget->axisRectCount() != 0) {
        if (mIsButtonPressed == false) {
            if (mPlotWidget->getIndexOfAxisRectByPoint(QPoint(mXCurrentPosition, mYCurrentPosition)) != -1) {
                QPainter painter(mPlotWidget);
                painter.setPen(QPen(mPlotWidget->mPlotStyle.mColorPref.mGraficalPrimitivesColor));
                painter.drawLine(mXCurrentPosition, 0, mXCurrentPosition, mPlotWidget->height());
            }
            return;
        }

        if ((mYStartZoom >= 0) && (mXStartZoom >= 0)){
            QPainter painter(mPlotWidget);
            painter.setPen(QPen(mPlotWidget->mPlotStyle.mColorPref.mGraficalPrimitivesColor));

            painter.drawLine(mXStartZoom, 0, mXStartZoom, mPlotWidget->height());
            painter.drawLine(mXCurrentPosition, 0, mXCurrentPosition, mPlotWidget->height());
            painter.drawLine(mXStartZoom, mYStartZoom, mXCurrentPosition, mYStartZoom);

            drawLineEnding(painter, QVector2D(mXStartZoom, mYStartZoom),
                           QVector2D((mXStartZoom <  mXCurrentPosition) ? -1 : 1, 0), QCPLineEnding::esSpikeArrow);
            drawLineEnding(painter, QVector2D(mXCurrentPosition, mYStartZoom),
                           QVector2D((mXStartZoom <  mXCurrentPosition) ? 1 : -1, 0), QCPLineEnding::esSpikeArrow);

        }

    }
}


void PlotWidgetFastSecondaryProcessingState::initializeState()
{
    PlotWidgetState::initializeState();
    clearState();
}

void PlotWidgetFastSecondaryProcessingState::clearState()
{
    mXStartZoom = -1;
    mYStartZoom = -1;
    mXCurrentPosition = -1;
    mYCurrentPosition = -1;
    mIsButtonPressed = false;
    mAxisIndexToZoom = -1;
    mZoomType = ZoomType::BoundedZoom;
}

