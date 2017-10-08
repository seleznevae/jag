#ifndef PLOTWIDGETWITHTOOLBAR_H
#define PLOTWIDGETWITHTOOLBAR_H

#include <QWidget>
#include <QMainWindow>
#include "fullScreen/QFullScreenAdapter.h"


class QToolBar;
class PlotWidget;

class PlotWidgetWithToolbar : public QMainWindow, public QFullScreenAdapter
{
    Q_OBJECT
public:
    explicit PlotWidgetWithToolbar(QWidget *parent = 0);

signals:

public slots:

public:
    QToolBar *mToolBar;
    PlotWidget *mPlotWidget;

};

#endif // PLOTWIDGETWITHTOOLBAR_H
