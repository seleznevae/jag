#ifndef TILEMANAGERWITHTOOLBAR_H
#define TILEMANAGERWITHTOOLBAR_H

#include <QWidget>
#include <QMainWindow>
#include "fullScreen/QFullScreenAdapter.h"

class WidgetTileManager;

class TileManagerWithToolbar : public QMainWindow, public QFullScreenAdapter
{
    Q_OBJECT
public:
    explicit TileManagerWithToolbar(QWidget *parent = 0);

signals:

public slots:
    void installToolbarActions();

public:
    QToolBar *mPlotToolBar;
    QToolBar *mZoomToolBar;
    QToolBar *mProcessingToolBar ;
    QToolBar *mOptionsToolBar;

    WidgetTileManager *mTileManager;
};

#endif // TILEMANAGERWITHTOOLBAR_H



