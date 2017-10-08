#ifndef CONTROLWIDGETWRAPPER_H
#define CONTROLWIDGETWRAPPER_H



#include "controlwrappers/widgetwrapperwithheader.h"


class ControlWidgetWrapper : public WidgetWrapperWithHeader// public QSint::ActionGroup
{
    Q_OBJECT
public:
    ControlWidgetWrapper(QWidget* controlWidget, QWidget *parent=0);
    virtual ~ControlWidgetWrapper() { }

public slots:

    void show();
    void treatHidingOfInternalWidget();



protected:
    virtual void resizeEvent(QResizeEvent * event);
    QWidget *mControlWidget;
};

#endif // CONTROLWIDGETWRAPPER_H





