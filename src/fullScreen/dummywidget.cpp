#include "dummywidget.h"
#include <QKeyEvent>
#include <QDebug>
//#include "core/plotwidget.h"
//#include "commonWidgets/plotwidgetwithtoolbar.h"
#include "commonWidgets/tilemanagerwithtoolbar.h"
#include "basic/basic.h"
#include "global/global_definitions.h"
#include "core/jagmainwindow.h"

DummyWidget::DummyWidget(QWidget *parent) :
    QWidget(parent)
{
}

void DummyWidget::keyPressEvent(QKeyEvent *event)
{
    event->ignore();
    if (event->key() == Qt::Key_F11) {
      QList<QObject*> childList = children();
      TileManagerWithToolbar* plot = qobject_cast<TileManagerWithToolbar*>(childList[1]);
      if (plot == NULL) {
          errorExit("Error in %s", __FUNCTION__);
      }
      plot->toogleFullScreen();
    } else {
        global::gJagMainWindowPointer->keyPressEvent(event);
    }

//        event->ignore();
//    QList<QObject*> childList = children();
//    PlotWidgetWithToolbar* plot = qobject_cast<PlotWidgetWithToolbar*>(childList[1]);
//    if (plot == NULL) {
//        errorExit("Error in %s", __FUNCTION__);
//    }
//    plot->toogleFullScreen();
}

//void DummyWidget::mousePressEvent(QMouseEvent *event)
//{
//    event->ignore();
//    QList<QObject*> childList = children();
//    PlotWidgetWithToolbar* plot = qobject_cast<PlotWidgetWithToolbar*>(childList[1]);
//    if (plot == NULL) {
//        errorExit("Error in %s", __FUNCTION__);
//    }
//    plot->toogleFullScreen();
//}
