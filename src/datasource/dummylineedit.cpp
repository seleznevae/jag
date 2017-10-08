#include "dummylineedit.h"
#include <QKeyEvent>

DummyLineEdit::DummyLineEdit(QWidget *parent) :
    QLineEdit(parent)
{
}

void DummyLineEdit::keyPressEvent(QKeyEvent *event)
{
    event->ignore();
}
