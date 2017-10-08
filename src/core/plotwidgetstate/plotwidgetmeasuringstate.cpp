#include "plotwidgetmeasuringstate.h"
#include <QPainter>
#include "global/global_definitions.h"
#include "basic/bassic.h"

PlotWidgetMeasuringState::PlotWidgetMeasuringState(PlotWidget *par)
    :PlotWidgetState(par), mXCurrentPosition(-1),
    mYCurrentPosition(-1),
    mXStartPosition(-1), mYStartPosition(-1),
    mIsButtonPressed(false), mAxisIndexToMeasure(-1)
{
}

void PlotWidgetMeasuringState::treatMousePressEvent(QMouseEvent *event)
{
    if (mPlotWidget->axisRectCount() != 0) {
        Qt::MouseButtons buttons = event->buttons();
        if (buttons &  Qt::LeftButton) {

//            auto newX =  event->x();
//            QRect axisRec= mPlotWidget->axisRect(0)->rect();
//            if ((newX > axisRec.x()) && (newX < axisRec.x() + axisRec.width())) {
//                const_cast<PlotLayoutModel*>(mPlotWidget->mLayoutModel)->addCommand(mPlotWidget->axisRect(0)->axis(QCPAxis::atBottom)->pixelToCoord(newX));
//            }
            mXStartPosition = event->x();
            mYStartPosition = event->y();

                double x_coord = event->x();
                double y_coord = event->y();

                mAxisIndexToMeasure = -1;
                for (int axisRectIndex = 0; axisRectIndex < mPlotWidget->axisRectCount(); ++axisRectIndex) {
                    QRect axisRec = mPlotWidget->axisRect(axisRectIndex)->rect();
                    if (((x_coord >= axisRec.x()) && (x_coord <= axisRec.x()+ axisRec.width()))
                        && ((y_coord >= axisRec.y()) && (y_coord <= axisRec.y()+ axisRec.height()))) {
                            mAxisIndexToMeasure = axisRectIndex;
                            break;
                    }
                }
                if (mAxisIndexToMeasure == -1) {
                    clearState();
                }

        } else if (buttons &  Qt::RightButton) {
            auto newX =  event->x();
            QRect axisRec= mPlotWidget->axisRect(0)->rect();
            if ((newX > axisRec.x()) && (newX < axisRec.x() + axisRec.width())) {
                const_cast<PlotLayoutModel*>(mPlotWidget->mLayoutModel)->addAnchor(mPlotWidget->axisRect(0)->axis(QCPAxis::atBottom)->pixelToCoord(newX));
            }
        }
    }
}

void PlotWidgetMeasuringState::treatMouseMoveEvent(QMouseEvent *event)
{
//    if (mPlotWidget->axisRectCount() != 0) {
//        double graphTracerKey = mPlotWidget->axisRect(0)->axis(QCPAxis::atBottom)->pixelToCoord(event->x());
//        for (auto graphTracer : mPlotWidget->mTracerVector) {
//            graphTracer->setGraphKey(graphTracerKey);
//        }
//        mPlotWidget->replot();

//        mXCurrentPosition = event->x();
//        mYCurrentPosition = event->y();
//        mPlotWidget->update();
//    }


//    if (mPlotWidget->axisRectCount() != 0) {

//        Qt::MouseButtons buttons = event->buttons();
//        if (((buttons &  Qt::LeftButton))
//        && (mAxisIndexToMeasure >= 0)) {
//            mIsButtonPressed = true;
//            mXCurrentPosition = event->x();
//            mYCurrentPosition = event->y();

//        } else {
//            mIsButtonPressed = false;
//        }
//        mPlotWidget->update();
//    }



    if (mPlotWidget->axisRectCount() != 0) {

        Qt::MouseButtons buttons = event->buttons();
        double graphTracerKey = NAN;
        if ((buttons &  Qt::LeftButton) && (mAxisIndexToMeasure >= 0)) {
            mIsButtonPressed = true;
            graphTracerKey = mPlotWidget->axisRect(0)->axis(QCPAxis::atBottom)->pixelToCoord(-1);
        } else {
            mIsButtonPressed = false;
            graphTracerKey = mPlotWidget->axisRect(0)->axis(QCPAxis::atBottom)->pixelToCoord(event->x());
        }
        for (auto graphTracer : mPlotWidget->mTracerVector) {
            graphTracer->setGraphKey(graphTracerKey);
        }
        mPlotWidget->replot();
        mXCurrentPosition = event->x();
        mYCurrentPosition = event->y();

        if (mAxisIndexToMeasure != -1) {
            QRect axisRec= mPlotWidget->axisRect(mAxisIndexToMeasure)->rect();
            if (mXCurrentPosition < axisRec.x())
                mXCurrentPosition = axisRec.x();
            if (mXCurrentPosition > axisRec.x() + axisRec.width())
                    mXCurrentPosition = axisRec.x() + axisRec.width();
            if (mYCurrentPosition < axisRec.y())
                mYCurrentPosition = axisRec.y();
            if (mYCurrentPosition > axisRec.y() + axisRec.height())
                mYCurrentPosition = axisRec.y() + axisRec.height();
        }

        mPlotWidget->update();


    }
}

void PlotWidgetMeasuringState::treatMouseReleaseEvent(QMouseEvent *event)
{
    clearState();
    mXCurrentPosition = event->x();
    mYCurrentPosition = event->y();
    mPlotWidget->update();
}

void PlotWidgetMeasuringState::treatPaintEvent(QPaintEvent */*event*/)
{

    if (mPlotWidget->axisRectCount() != 0) {
        int &xCurrent = mXCurrentPosition;
        int &yCurrent = mYCurrentPosition;

        QPainter painter(mPlotWidget);

        auto brush(mPlotWidget->mLayoutModel->plotStyle().mColorPref.mBackgroundColor);
        auto draw_text = [&painter, brush] (int x1, int y1, int x2, int y2, const QString &text) {
            painter.save();
//            painter.setBrush(global::gDefaultPlotBackgroundColor);
            painter.setBrush(brush);
            painter.setPen(Qt::NoPen);
            QFontMetrics fontMetrics(painter.font());
            int width = fontMetrics.width(text)/2;
            int height = fontMetrics.height()/2;

            painter.drawRect(QRectF(QPointF((x1+x2)/2-width, (y1+y2)/2-height),
                                    QPointF((x1+x2)/2+width, (y1+y2)/2+height)));
            painter.restore();
            painter.drawText(QRectF(QPointF(x1, y1), QPointF(x2, y2))
                             ,  Qt::AlignCenter, text);

        };

        if (mIsButtonPressed == false) {
            for (int axisRectIndex = 0; axisRectIndex < mPlotWidget->axisRectCount(); ++axisRectIndex) {
                QRect axisRec = mPlotWidget->axisRect(axisRectIndex)->rect();
                if (((xCurrent >= axisRec.x()) && (xCurrent <= axisRec.x()+ axisRec.width()))
                    && ((yCurrent >= axisRec.y()) && (yCurrent <= axisRec.y()+ axisRec.height()))) {

                        painter.setPen(QPen(mPlotWidget->mPlotStyle.mColorPref.mGraficalPrimitivesColor));
                        painter.drawLine(xCurrent, 0, xCurrent, mPlotWidget->height());
                        painter.drawLine(0, yCurrent, mPlotWidget->width(), yCurrent);

                        double xOut = mPlotWidget->axisRect(axisRectIndex)->axis(QCPAxis::atBottom)->pixelToCoord(xCurrent);
                        double yOut = mPlotWidget->axisRect(axisRectIndex)->axis(QCPAxis::atLeft)->pixelToCoord(yCurrent);
                        emit currentCoordinates(xOut, yOut);

                        if (global::gDrawValuesInMeasuringMode) {
                            QString xText = "x = " + QString::number(xOut, 'f', global::gDoubleToStringConversionPrecision);
                            QString yText = "y = " + QString::number(yOut, 'f', global::gDoubleToStringConversionPrecision);

                            QFont newFont = QApplication::font();
                            newFont.setPointSize(3 + newFont.pointSize());
                            painter.setFont(newFont);
                            QFontMetrics metrics(newFont);
                            int xTextWidth  = metrics.width(xText) + 2;
                            int xTextHeight = metrics.height() + 2;
                            int yTextWidth  = metrics.width(yText) + 2;
                            int yTextHeight = metrics.height() + 2;
                            int gap = 20; //distance between cursor and rectangular with values

                            if ((xCurrent + xTextWidth + gap < mPlotWidget->width())
                                    && (xCurrent + yTextWidth + gap < mPlotWidget->width())
                                    && (yCurrent - yTextHeight - xTextHeight - gap > 0)) {

                                //xtext at top, yText at bottom
//                                draw_text(xCurrent + gap, yCurrent - yTextHeight - xTextHeight - gap,
//                                          xCurrent + gap + xTextWidth, yCurrent - yTextHeight - gap, xText);
//                                draw_text(xCurrent + gap, yCurrent - yTextHeight -  gap,
//                                          xCurrent + gap + yTextWidth, yCurrent - gap, yText);

                                //yText at top, xText at bottom
                                draw_text(xCurrent + gap, yCurrent - xTextHeight - gap,
                                          xCurrent + gap + xTextWidth, yCurrent - gap, xText);
                                draw_text(xCurrent + gap, yCurrent - yTextHeight - xTextHeight -  gap,
                                          xCurrent + gap + yTextWidth, yCurrent- xTextHeight - gap, yText);

                            } else {
                                //xtext at top, yText at bottom
//                                draw_text(xCurrent - gap - xTextWidth, yCurrent + gap,
//                                          xCurrent - gap, yCurrent + xTextHeight + gap, xText);
//                                draw_text(xCurrent - gap - yTextWidth, yCurrent + xTextHeight + gap,
//                                          xCurrent - gap, yCurrent + xTextHeight + yTextHeight + gap, yText);

                                //yText at top, xText at bottom
                                draw_text(xCurrent - gap - xTextWidth, yCurrent + yTextHeight + gap,
                                          xCurrent - gap, yCurrent + xTextHeight + yTextHeight + gap, xText);
                                draw_text(xCurrent - gap - yTextWidth, yCurrent + gap,
                                          xCurrent - gap, yCurrent + yTextHeight + gap, yText);
                            }
                        }

                        drawTracers(painter);
                        break;
                }
            }
        }

        if (mIsButtonPressed == true) {
            if (mPlotWidget->getIndexOfAxisRectByPoint(QPoint(mXCurrentPosition, mYCurrentPosition)) != -1) {
                painter.setPen(QPen(mPlotWidget->mPlotStyle.mColorPref.mGraficalPrimitivesColor));

                double xStart = mPlotWidget->axisRect(mAxisIndexToMeasure)->axis(QCPAxis::atBottom)->pixelToCoord(mXStartPosition);
                double xCurrent = mPlotWidget->axisRect(mAxisIndexToMeasure)->axis(QCPAxis::atBottom)->pixelToCoord(mXCurrentPosition);
                QString deltaX = valueToString(xCurrent - xStart, GraphModel::StringRepresentation::Decimal);
                double yStart = mPlotWidget->axisRect(mAxisIndexToMeasure)->axis(QCPAxis::atLeft)->pixelToCoord(mYStartPosition);
                double yCurrent = mPlotWidget->axisRect(mAxisIndexToMeasure)->axis(QCPAxis::atLeft)->pixelToCoord(mYCurrentPosition);
                QString deltaY = valueToString(yCurrent - yStart, GraphModel::StringRepresentation::Decimal);

                QFont newFont = QApplication::font();
                newFont.setPointSize(3 + newFont.pointSize());
                painter.setFont(newFont);
                QFontMetrics metrics(newFont);
                int xTextWidth  = metrics.width(deltaX) + 2;
                int xTextHeight = metrics.height() + 2;
                int yTextWidth  = metrics.width(deltaY) + 2;
                int yTextHeight = metrics.height() + 2;

                painter.drawLine(0, mYCurrentPosition, mPlotWidget->width(), mYCurrentPosition);
                painter.drawLine(mXStartPosition, 0, mXStartPosition, mPlotWidget->height());

                if (xTextWidth < abs(mXStartPosition - mXCurrentPosition)) {
                    if (mYStartPosition > mYCurrentPosition || mYStartPosition < xTextHeight)
                        draw_text(mXStartPosition, mYStartPosition, mXCurrentPosition, mYStartPosition+xTextHeight, deltaX);
                    else
                        draw_text(mXStartPosition, mYStartPosition-xTextHeight, mXCurrentPosition, mYStartPosition, deltaX);

                } else {
                    if (mXStartPosition < mXCurrentPosition) {
                        if (mYStartPosition > mYCurrentPosition || mYStartPosition < xTextHeight)
                            draw_text(mXCurrentPosition-xTextWidth, mYStartPosition, mXCurrentPosition, mYStartPosition+xTextHeight, deltaX);
                        else
                            draw_text(mXCurrentPosition-xTextWidth, mYStartPosition-xTextHeight, mXCurrentPosition, mYStartPosition, deltaX);
                    } else {
                        if (mYStartPosition > mYCurrentPosition || mYStartPosition < xTextHeight)
                            draw_text(mXCurrentPosition, mYStartPosition, mXCurrentPosition + xTextWidth, mYStartPosition+xTextHeight, deltaX);
                        else
                            draw_text(mXCurrentPosition, mYStartPosition-xTextHeight, mXCurrentPosition + xTextWidth, mYStartPosition, deltaX);
                    }
                }

                if (yTextHeight < abs(mYStartPosition - mYCurrentPosition)) {
                    if (mXStartPosition < mXCurrentPosition) {
                        if (mXCurrentPosition + yTextWidth < mPlotWidget->width()) {
                            draw_text(mXCurrentPosition, mYStartPosition, mXCurrentPosition + yTextWidth, mYCurrentPosition, deltaY);
                        } else {
                            draw_text(mXCurrentPosition - yTextWidth, mYStartPosition, mXCurrentPosition, mYCurrentPosition, deltaY);
                        }
                    }
                    else
                        draw_text(mXCurrentPosition-yTextWidth, mYStartPosition, mXCurrentPosition, mYCurrentPosition, deltaY);

                } else {
                    if (mYStartPosition < mYCurrentPosition) {
                        if (mXStartPosition < mXCurrentPosition)
                            draw_text(mXCurrentPosition, mYCurrentPosition - yTextHeight, mXCurrentPosition + yTextWidth, mYCurrentPosition, deltaY);
                        else
                            draw_text(mXCurrentPosition-yTextWidth, mYCurrentPosition - yTextHeight, mXCurrentPosition, mYCurrentPosition, deltaY);
                    } else {
                        if (mXStartPosition < mXCurrentPosition)
                            draw_text(mXCurrentPosition, mYCurrentPosition + yTextHeight, mXCurrentPosition + yTextWidth, mYCurrentPosition, deltaY);
                        else
                            draw_text(mXCurrentPosition-yTextWidth, mYCurrentPosition + yTextHeight, mXCurrentPosition, mYCurrentPosition, deltaY);
                    }
                }

//                painter.drawLine(0, mYCurrentPosition, mPlotWidget->width(), mYCurrentPosition);
//                painter.drawLine(mXStartPosition, 0, mXStartPosition, mPlotWidget->height());



                drawArrow(painter, QVector2D(mXStartPosition, mYStartPosition), QVector2D(mXCurrentPosition, mYStartPosition),
                          QCPLineEnding::esSpikeArrow, QCPLineEnding::esSpikeArrow);
                drawArrow(painter, QVector2D(mXCurrentPosition, mYStartPosition), QVector2D(mXCurrentPosition, mYCurrentPosition),
                          QCPLineEnding::esSpikeArrow, QCPLineEnding::esSpikeArrow);

            }
            return;
        }
    }


}

void PlotWidgetMeasuringState::treatLeaveEvent(QEvent *)
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

void PlotWidgetMeasuringState::initializeState()
{
    setInteractions();
    setCursorShape();
    mPlotWidget->updateTracers();
}

void PlotWidgetMeasuringState::setCursorShape()
{
    mPlotWidget->setCursor(Qt::CrossCursor);
}

void PlotWidgetMeasuringState::clearState()
{
    mXStartPosition = -1;
    mYStartPosition = -1;
    mXCurrentPosition = -1;
    mYCurrentPosition = -1;
    mIsButtonPressed = false;
    mAxisIndexToMeasure = -1;
    QApplication::restoreOverrideCursor();
}

void PlotWidgetMeasuringState::drawTracers(QPainter &painter)
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
