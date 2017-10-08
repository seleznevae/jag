#ifndef EVENTCONTROLWIDGETWRAPPER_H
#define EVENTCONTROLWIDGETWRAPPER_H

#include "controlwrappers/controlwidgetwrapper.h"
#include "controlWidgets/eventcontrolwidget.h"

class EventControlWidgetWrapper: public ControlWidgetWrapper
{
    Q_OBJECT
public:
    explicit EventControlWidgetWrapper(QWidget *parent=0);
    ~EventControlWidgetWrapper() { }

    void setTimeEvent(TimeEvent *newEvent);

public slots:
    void treatNameChanging();
    void treatTimeChanging();
    void treatDescriptionChanging();
    void treatEventDeleting(QObject* event);
    void treatLabelDeviationChanging();

//    void treatBrushColorChanging();
//    void showBrushColorDialog();
    void treatBrushStyleChanging();
    void treatBrushBasisChanging();



signals:
    void arrowedTextCreationRequest(double);



private:
    EventControlWidget *mControlWidgetCopy;
};

#endif // EVENTCONTROLWIDGETWRAPPER_H

