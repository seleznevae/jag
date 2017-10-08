#include "ipushbutton.h"
#include <QMouseEvent>
#include <QDebug>

//IPushButton::IPushButton(QWidget *parent) :
//    QPushButton(parent)
//{
//}

void IPushButton::setHierarchyWidget(PlotHierarchyWidget *hierarchyWidget)
{
    mHierarchyWidget = hierarchyWidget;
}

//void IPushButton::mousePressEvent(QMouseEvent *event)
//{
//    qDebug() << "mouse press in IPushButton --------";
////    click();
//    mHierarchyWidget->treatDeletingRequest();
////    event->ignore();
//    QPushButton::mousePressEvent(event);
//}

void IPushButton::enterEvent(QEvent *event)
{
//    qDebug() << "mouse enter in IPushButton --------";
//    click();
    mHierarchyWidget->saveActivatedItems();
//    event->ignore();
    QToolButton/*QPushButton*/::enterEvent(event);
}
