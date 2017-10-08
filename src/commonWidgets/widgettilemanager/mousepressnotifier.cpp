#include "mousepressnotifier.h"
#include <QEvent>
#include <QWidget>
#include <QCoreApplication>

MousePressNotifier::MousePressNotifier(QObject *parent) : QObject(parent)
{

}

bool MousePressNotifier::eventFilter(QObject *watched, QEvent *event)
{
    auto type = event->type();
    if (type == QEvent::MouseButtonPress) {
        auto widget = qobject_cast<QWidget*>(watched);
        if (widget) {
            emit clickOnWidget(widget);
        }
    }

    if (type == QEvent::MouseButtonDblClick
            || type == QEvent::MouseButtonPress
            || type == QEvent::MouseButtonRelease
            || type == QEvent::MouseMove) {
        qApp->sendEvent(parent(), event);
    }
    return false;
}

