#include "messagelabel.h"
#include "global/global_definitions.h"
#include <QDebug>

MessageLabel::MessageLabel(QWidget *parent) :
    QLabel(parent), mTimerId(0)
{
//    connect(gJagMainWindowPointer, SIGNAL(widthChanged(int)),
//            this, SLOT(setLabelWidth(int)));
}



void MessageLabel::setMessage(const QString &message, int interval)
{
    setText(message);
//    qDebug() << text();
    if (mTimerId != 0)
        killTimer(mTimerId);
    mTimerId = startTimer(interval);
}

void MessageLabel::setLabelWidth(int wid)
{
//    qDebug() << "width ---> " << wid;
    setMaximumWidth(wid - 40);
//    setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred);

}

void MessageLabel::connectToJagMainWindow(JagMainWindow *jagWindow)
{
    connect(jagWindow, SIGNAL(widthChanged(int)),
            this, SLOT(setLabelWidth(int)));
}



void MessageLabel::timerEvent(QTimerEvent */*event*/)
{
    setText("");
    if (mTimerId != 0) {
        killTimer(mTimerId);
        mTimerId = 0;
    }
}
