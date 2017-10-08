#ifndef MESSAGELABEL_H
#define MESSAGELABEL_H

#include <QLabel>
#include "core/jagmainwindow.h"

class JagMainWindow;

/*!
 \brief Widget used in status bar of JagMainWindow.

 As MessageLabel is a subclass of QLabel it can show text in different colors.

*/
class MessageLabel : public QLabel
{
    Q_OBJECT
public:
    explicit MessageLabel(QWidget *parent = 0);

signals:

public slots:
    void setMessage(const QString & message, int interval = 100000);
    void setLabelWidth(int width);
    void connectToJagMainWindow(JagMainWindow* jagWindow);

protected:
    virtual void timerEvent(QTimerEvent *event);
private:
    int mTimerId;
//    JagMainWindow* mJagWindow;
};

#endif // MESSAGELABEL_H
