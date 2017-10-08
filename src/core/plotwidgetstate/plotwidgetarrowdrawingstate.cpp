#include "plotwidgetarrowdrawingstate.h"
#include "basic/bassic.h"
#include "global/global_definitions.h"

PlotWidgetArrowDrawingState::PlotWidgetArrowDrawingState(PlotWidget *par)
    :PlotWidgetState(par), mXStart(-1), mYStart(-1),
    mXCurrentPosition(-1), mYCurrentPosition(-1),
    mIsButtonPressed(false), mAxisIndex(-1), mArrowType(ArrowType::Internal)

{

}

void PlotWidgetArrowDrawingState::treatMousePressEvent(QMouseEvent *event)
{
    if (mPlotWidget->axisRectCount() != 0) {
        mXStart = event->x();
        mYStart = event->y();

        QApplication::setOverrideCursor(Qt::CrossCursor);

            double x_coord = event->x();
            double y_coord = event->y();

            mAxisIndex = -1;
            for (int axisRectIndex = 0; axisRectIndex < mPlotWidget->axisRectCount(); ++axisRectIndex) {
                QRect axisRec = mPlotWidget->axisRect(axisRectIndex)->rect();
                if (((x_coord >= axisRec.x()) && (x_coord <= axisRec.x()+ axisRec.width()))
                    && ((y_coord >= axisRec.y()) && (y_coord <= axisRec.y()+ axisRec.height()))) {
                        mAxisIndex= axisRectIndex;
                        break;
                }
            }

            if (mAxisIndex == -1) {
                clearState();
            }
    }
}

void PlotWidgetArrowDrawingState::treatMouseReleaseEvent(QMouseEvent *event)
{
    QApplication::restoreOverrideCursor();

    if (mPlotWidget->axisRectCount() != 0) {

        if (mAxisIndex != -1) {
            auto newY =  event->y();
            auto newX =  event->x();

            double xStart = mPlotWidget->axisRect(mAxisIndex)->axis(QCPAxis::atBottom)->pixelToCoord(mXStart);
            double yStart = mPlotWidget->axisRect(mAxisIndex)->axis(QCPAxis::atLeft)->pixelToCoord(mYStart);
            double xEnd = mPlotWidget->axisRect(mAxisIndex)->axis(QCPAxis::atBottom)->pixelToCoord(newX);
            double yEnd = mPlotWidget->axisRect(mAxisIndex)->axis(QCPAxis::atLeft)->pixelToCoord(newY - 1);


            auto visibleAxisVector = mPlotWidget->mLayoutModel->getVisibleAxisModelVector();
            if (mAxisIndex < visibleAxisVector.size()) {
                ArrowedText *newArrowedText = new ArrowedText;
                newArrowedText->setText("y = " + valueToString(yEnd, GraphModel::StringRepresentation::Decimal));
                newArrowedText->setTextCenter(QPointF(xStart, yStart));
                newArrowedText->setArrowFinish(QPointF(xEnd, yEnd));
                visibleAxisVector[mAxisIndex]->addArrowedText(newArrowedText);
            }
            mIsButtonPressed = false;
        }
    }

    clearState();
}

void PlotWidgetArrowDrawingState::treatMouseMoveEvent(QMouseEvent *event)
{
    if (mPlotWidget->axisRectCount() != 0) {

        Qt::MouseButtons buttons = event->buttons();
        if ((buttons &  Qt::LeftButton) || (buttons &  Qt::RightButton)) {
            if (buttons &  Qt::LeftButton)
                mArrowType = ArrowType::Internal;
            else if (buttons &  Qt::RightButton)
                mArrowType = ArrowType::External;

            mIsButtonPressed = true;

            if (mAxisIndex != -1) {
                mXCurrentPosition = event->x();
                mYCurrentPosition = event->y();

                QRect axisRec = mPlotWidget->axisRect(0)->rect();
                if (mXCurrentPosition < axisRec.x())
                    mXCurrentPosition = axisRec.x();
                if (mXCurrentPosition > axisRec.x() + axisRec.width())
                        mXCurrentPosition = axisRec.x() + axisRec.width();

                QRect axisRecY= mPlotWidget->axisRect(mAxisIndex)->rect();

                if (mYCurrentPosition < axisRecY.y())
                    mYCurrentPosition = axisRecY.y();
                if (mYCurrentPosition > axisRecY.y() + axisRecY.height())
                    mYCurrentPosition = axisRecY.y() + axisRecY.height();
            }

        } else {
            mIsButtonPressed = false;
        }
        mPlotWidget->update();
    }
}

void PlotWidgetArrowDrawingState::treatPaintEvent(QPaintEvent *)
{
    if (mPlotWidget->axisRectCount() != 0) {
        if (mXStart >= 0 && mYStart >= 0 && mXCurrentPosition >= 0 && mYCurrentPosition >= 0){
                QPainter painter(mPlotWidget);
                painter.setPen(QPen(mPlotWidget->mPlotStyle.mArrowedTextProperties.mPen));
                drawArrow(painter, QVector2D(mXStart, mYStart), QVector2D(mXCurrentPosition, mYCurrentPosition),
                          mPlotWidget->mPlotStyle.mArrowedTextProperties.mArrowEndingStyle);

        }
    }
}

void PlotWidgetArrowDrawingState::initializeState()
{
    PlotWidgetState::initializeState();
    clearState();
}

void PlotWidgetArrowDrawingState::clearState()
{
    mXStart = -1;
    mYStart = -1;
    mXCurrentPosition = -1;
    mYCurrentPosition = -1;
    mIsButtonPressed = false;
    mAxisIndex = -1;
    mArrowType = ArrowType::Internal;
}
