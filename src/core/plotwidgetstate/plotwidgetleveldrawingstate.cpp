#include "core/plotwidgetstate/plotwidgetleveldrawingstate.h"
#include <QPainter>
#include "global/global_definitions.h"

PlotWidgetLevelDrawingState::PlotWidgetLevelDrawingState(PlotWidget *par)
    :PlotWidgetState(par), mXCurrentPosition(-1),
    mYCurrentPosition(-1)
{
}

void PlotWidgetLevelDrawingState::treatMousePressEvent(QMouseEvent *event)
{
    if (mPlotWidget->axisRectCount() != 0) {
        Qt::MouseButtons buttons = event->buttons();
        if (buttons &  Qt::LeftButton) {
            auto newX =  event->x();
            QRect axisRec= mPlotWidget->axisRect(0)->rect();
            if ((newX > axisRec.x()) && (newX < axisRec.x() + axisRec.width())) {
                const_cast<PlotLayoutModel*>(mPlotWidget->mLayoutModel)->addCommand(mPlotWidget->axisRect(0)->axis(QCPAxis::atBottom)->pixelToCoord(newX));
            }
        } else if (buttons &  Qt::RightButton) {
//            auto newX =  event->x();
//            QRect axisRec= mPlotWidget->axisRect(0)->rect();
//            if ((newX > axisRec.x()) && (newX < axisRec.x() + axisRec.width())) {

//                const_cast<PlotLayoutModel*>(mPlotWidget->mLayoutModel)->addAnchor(mPlotWidget->axisRect(0)->axis(QCPAxis::atBottom)->pixelToCoord(newX));
//            }
            auto newY =  event->y();
            int axisIndex = mPlotWidget->getIndexOfAxisRectByPoint(event->pos());
            QVector<AxisModel *> axisVector = mPlotWidget->mLayoutModel->getVisibleAxisModelVector();
            if (axisIndex >= 0 && axisIndex < axisVector.size() && axisIndex < mPlotWidget->axisRectCount()) {
                axisVector[axisIndex]->addHorizontalLevelByLevel(mPlotWidget->axisRect(axisIndex)->axis(QCPAxis::atLeft)->pixelToCoord(newY));
            }
        }
    }
}

void PlotWidgetLevelDrawingState::treatMouseMoveEvent(QMouseEvent *event)
{
    if (mPlotWidget->axisRectCount() != 0) {
        double graphTracerKey = mPlotWidget->axisRect(0)->axis(QCPAxis::atBottom)->pixelToCoord(event->x());
        for (auto graphTracer : mPlotWidget->mTracerVector) {
            graphTracer->setGraphKey(graphTracerKey);
        }
        mPlotWidget->replot();

        mXCurrentPosition = event->x();
        mYCurrentPosition = event->y();
        mPlotWidget->update();
    }
}

void PlotWidgetLevelDrawingState::treatPaintEvent(QPaintEvent */*event*/)
{

    if (mPlotWidget->axisRectCount() != 0) {
        int &xCurrent = mXCurrentPosition;
        int &yCurrent = mYCurrentPosition;

        for (int axisRectIndex = 0; axisRectIndex < mPlotWidget->axisRectCount(); ++axisRectIndex) {
            QRect axisRec = mPlotWidget->axisRect(axisRectIndex)->rect();
            if (((xCurrent >= axisRec.x()) && (xCurrent <= axisRec.x()+ axisRec.width()))
                && ((yCurrent >= axisRec.y()) && (yCurrent <= axisRec.y()+ axisRec.height()))) {
                    QPainter painter(mPlotWidget);
                    painter.setPen(QPen(mPlotWidget->mPlotStyle.mColorPref.mGraficalPrimitivesColor));
                    painter.drawLine(xCurrent, 0, xCurrent, mPlotWidget->height());
                    painter.drawLine(0, yCurrent, mPlotWidget->width(), yCurrent);
                    double xOut = mPlotWidget->axisRect(axisRectIndex)->axis(QCPAxis::atBottom)->pixelToCoord(xCurrent);
                    double yOut = mPlotWidget->axisRect(axisRectIndex)->axis(QCPAxis::atLeft)->pixelToCoord(yCurrent);
                    emit currentCoordinates(xOut, yOut);
                    drawTracers(painter);
                    break;
            }
        }
    }
}

void PlotWidgetLevelDrawingState::treatLeaveEvent(QEvent *)
{
    //to remove graphical stuff in measuring mode
    if (mPlotWidget->axisRectCount() != 0) {
        double graphTracerKey = mPlotWidget->axisRect(0)->axis(QCPAxis::atBottom)->pixelToCoord(-1);
        for (auto graphTracer : mPlotWidget->mTracerVector) {
            graphTracer->setGraphKey(graphTracerKey);
        }
        mPlotWidget->replot();
        mXCurrentPosition = -1;
        mYCurrentPosition = -1;
        mPlotWidget->update();
    }
}

void PlotWidgetLevelDrawingState::initializeState()
{
    setInteractions();
    setCursorShape();
    mPlotWidget->updateTracers();
}

void PlotWidgetLevelDrawingState::setCursorShape()
{
    mPlotWidget->setCursor(Qt::CrossCursor);
}

void PlotWidgetLevelDrawingState::drawTracers(QPainter &painter)
{   //TODO: implement tracer painting for different styles and do it in a separate member function
    painter.save();
    QPen pen(Qt::red);
    pen.setWidth(1.);
    painter.setPen(pen);
    QBrush brush(Qt::white);
    painter.setBrush(brush);
    auto axisModelVector = mPlotWidget->mLayoutModel->getVisibleAxisModelVector();
    for (int axisIndex = 0; axisIndex < axisModelVector.size(); ++axisIndex) {
        double x_value = mPlotWidget->axisRect(axisIndex)->axis(QCPAxis::atBottom)->pixelToCoord(mXCurrentPosition);
        auto graphModelVector = axisModelVector[axisIndex]->getFunctionModelVector();
        for (int modelIndex = 0; modelIndex < graphModelVector.size(); ++modelIndex) {
            double y_value = graphModelVector[modelIndex]->getValue(x_value);
            int xInWidgetCoord = mXCurrentPosition;
            int yInWidgetCoord = mPlotWidget->axisRect(axisIndex)->axis(QCPAxis::atLeft)->coordToPixel(y_value);
            painter.drawEllipse(QPoint(xInWidgetCoord, yInWidgetCoord), global::gTracerSize/2, global::gTracerSize/2);
        }
    }
    painter.restore();
}
