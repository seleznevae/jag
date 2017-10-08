#include "plotwidgetstate.h"

PlotWidgetState::PlotWidgetState(PlotWidget *par)
    :QObject(par), mPlotWidget(par)
{
    connectToPlotWidget();
}

void PlotWidgetState::setPlotWidget(PlotWidget *widget)
{
    connectToPlotWidget();
    setParent(widget);
    mPlotWidget = widget;
    disconnectFromPlotWidget();
}

void PlotWidgetState::initializeState()
{
    setInteractions();
    setCursorShape();

    for (int i = 0; i < mPlotWidget->mTracerVector.size(); ++i) {
        mPlotWidget->removeItem(mPlotWidget->mTracerVector[i]);
        //removeItem will call delete and clear QCustomPlot internal containers, so the stuff below is supposed to be excessive
//            mTracerVector[i]->setParent(NULL);
//            delete mTracerVector[i];
    }
    mPlotWidget->mTracerVector.clear();

}

void PlotWidgetState::setInteractions()
{
    mPlotWidget->setInteractions(0);
}

void PlotWidgetState::setCursorShape()
{
    mPlotWidget->setCursor(Qt::ArrowCursor);
}


void PlotWidgetState::drawLineEnding(QPainter &qPainter, const QVector2D &point, const QVector2D &direction, QCPLineEnding lineEnding)
{
    QCPPainter qcpPainter(qPainter.device());
    qcpPainter.setPen(qPainter.pen());
    QCPLineEnding le(lineEnding);
    le.draw(&qcpPainter, point, direction);
}

void PlotWidgetState::drawArrow(QPainter &qPainter, const QVector2D &start, const QVector2D &finish, QCPLineEnding lineEnding, QCPLineEnding lineStarting)
{

    QCPPainter qcpPainter(qPainter.device());
    qcpPainter.setPen(qPainter.pen());
    qPainter.drawLine(QPointF(start.x(), start.y()), QPointF(finish.x(), finish.y()));

    drawLineEnding(qPainter, finish, finish - start, lineEnding);
    if (lineStarting.style() != QCPLineEnding::esNone) {
        drawLineEnding(qPainter, start, start - finish, lineStarting);
    }

}

void PlotWidgetState::connectToPlotWidget()
{
    if (mPlotWidget) {
        connect(this, SIGNAL(xZoomRequest(double, double)),
                mPlotWidget, SIGNAL(xZoomRequest(double, double)));
        connect(this, SIGNAL(yZoomRequest(double, double, int)),
                mPlotWidget, SIGNAL(yZoomRequest(double, double, int)));
        connect(this, SIGNAL(xyZoomRequest(double, double, double, double, int)),
                mPlotWidget, SIGNAL(xyZoomRequest(double, double, double, double, int)));
        connect(this, SIGNAL(currentCoordinates(double, double)),
                mPlotWidget, SIGNAL(currentCoordinates(double, double)));
    }

}

void PlotWidgetState::disconnectFromPlotWidget()
{
    if (mPlotWidget) {
        disconnect(this, SIGNAL(xZoomRequest(double, double)),
                mPlotWidget, SIGNAL(xZoomRequest(double, double)));
        disconnect(this, SIGNAL(yZoomRequest(double, double, int)),
                mPlotWidget, SIGNAL(yZoomRequest(double, double, int)));
        disconnect(this, SIGNAL(xyZoomRequest(double, double, double, double, int)),
                mPlotWidget, SIGNAL(xyZoomRequest(double, double, double, double, int)));
        disconnect(this, SIGNAL(currentCoordinates(double, double)),
                mPlotWidget, SIGNAL(currentCoordinates(double, double)));
    }
}


