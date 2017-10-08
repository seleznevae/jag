#include "popupwidget.h"
#include <QApplication>
#include <QLayout>
#include <QDesktopWidget>
#include <QVBoxLayout>

PopupWidget::PopupWidget(QWidget *parent)
    :QFrame(parent, Qt::Popup), mWidget(nullptr)
{
    setAttribute(Qt::WA_WindowPropagation);
    if (parentWidget() == nullptr)
        setAttribute(Qt::WA_DeleteOnClose);
    setLayout(new QVBoxLayout());
    layout()->setSpacing(0);
    layout()->setMargin(0);
}

void PopupWidget::setWidget(QWidget *widget)
{
    if (mWidget) {
        layout()->removeWidget(mWidget);
        mWidget->setParent(0);
        delete mWidget;
    }
    mWidget = widget;
    if (mWidget) {
        mWidget->setParent(this);
        layout()->addWidget(mWidget);
    }
}

void PopupWidget::show(QPoint coord)
{
    QFrame::show();

    QRect space(QApplication::desktop()->availableGeometry());
    if (coord.x() < space.x())
        coord.setX(space.x());
    if (coord.y() < space.y())
        coord.setY(space.y());
    if (coord.x() > (space.right()-width()))
        coord.setX(space.right()-width());
    if (coord.y() > (space.bottom()-height()))
        coord.setY(space.bottom()-height());
    move(coord);
}

