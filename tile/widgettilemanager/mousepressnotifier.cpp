#include "mousepressnotifier.h"
#include <QEvent>
#include <QWidget>

MousePressNotifier::MousePressNotifier(QObject *parent) : QObject(parent)
{

}

bool MousePressNotifier::eventFilter(QObject *watched, QEvent *event)
{
    if (event->type() == QEvent::MouseButtonPress) {
        auto widget = qobject_cast<QWidget*>(watched);
        if (widget) {
            emit clickOnWidget(widget);
        }
    }
    return false;
}

