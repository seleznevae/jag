#ifndef PLOTWIDGETSTATE_H
#define PLOTWIDGETSTATE_H


#include <QObject>
#include "core/plotwidget.h"
#include "core/qcustomplot.h"
#include <QPainter>

class PlotWidgetState: QObject
{
    Q_OBJECT
public:
    explicit PlotWidgetState(PlotWidget *par = NULL);

    void setPlotWidget(PlotWidget *widget);


    //all virtual event handlers intentionally left blank, derived classes could redefine them if necessary
    virtual void treatMousePressEvent   (QMouseEvent    *) { }
    virtual void treatMouseReleaseEvent (QMouseEvent    *) { }
    virtual void treatMouseMoveEvent    (QMouseEvent    *) { }
    virtual void treatWheelEvent        (QWheelEvent    *) { }
    virtual void treatPaintEvent        (QPaintEvent    *) { }
    virtual void treatResizeEvent       (QResizeEvent   *) { }
    virtual void treatLeaveEvent        (QEvent         *) { }
    virtual void treatDropEvent         (QDropEvent     *) { }
    virtual void treatDragMoveEvent     (QDragMoveEvent *) { }
    virtual void treatDragLeaveEvent    (QDragLeaveEvent*) { }
    virtual void treatDragEnterEvent    (QDragEnterEvent*) { }


    virtual void initializeState();
    virtual void setInteractions();
    virtual void setCursorShape();



signals:
    void xZoomRequest(double, double);
    void yZoomRequest(double, double, int);
    void xyZoomRequest(double, double, double, double, int);
    void currentCoordinates(double, double);


protected:
    PlotWidget* mPlotWidget;


protected:
    static void drawLineEnding(QPainter &qPainter, const QVector2D &point, const QVector2D &direction, QCPLineEnding lineEnding);
    static void drawArrow(QPainter &qPainter, const QVector2D &start, const QVector2D &finish, QCPLineEnding lineEnding, QCPLineEnding lineStarting = QCPLineEnding::esNone);

private:
    void connectToPlotWidget();
    void disconnectFromPlotWidget();
};

//void PlotWidgetState::treatDragEnterEvent(QDragEnterEvent *) { }

#endif // PLOTWIDGETSTATE_H
