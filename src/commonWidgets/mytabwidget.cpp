#include "mytabwidget.h"
#include <QTabBar>
#include <QPainter>

MyTabWidget::MyTabWidget(QWidget *parent) :
    QTabWidget(parent)
{
    QObject::connect(this->tabBar(),  SIGNAL(tabMoved(int, int)),
                     this, SIGNAL(tabMoved(int, int)));
}

void MyTabWidget::paintEvent(QPaintEvent *event)
{
    QTabWidget::paintEvent(event);
}


