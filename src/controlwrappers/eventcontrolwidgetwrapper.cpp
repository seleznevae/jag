#include "eventcontrolwidgetwrapper.h"
#include <assert.h>


EventControlWidgetWrapper::EventControlWidgetWrapper(QWidget *parent)
    :ControlWidgetWrapper(new EventControlWidget(), parent), mControlWidgetCopy(NULL)
{
    mControlWidgetCopy = dynamic_cast<EventControlWidget*>(mControlWidget);
    assert(mControlWidgetCopy != NULL);

    setHeaderText(tr("Event properties"));

    connect(mControlWidgetCopy, SIGNAL(widgetIsHiding()),
            this, SLOT(treatHidingOfInternalWidget()));
    connect(mControlWidgetCopy, SIGNAL(arrowedTextCreationRequest(double)),
            this, SIGNAL(arrowedTextCreationRequest(double)));

}

void EventControlWidgetWrapper::setTimeEvent(TimeEvent *newEvent)
{
    mControlWidgetCopy->setTimeEvent(newEvent);
}

void EventControlWidgetWrapper::treatNameChanging()
{
    mControlWidgetCopy->treatNameChanging();
}

void EventControlWidgetWrapper::treatTimeChanging()
{
    mControlWidgetCopy->treatTimeChanging();
}

void EventControlWidgetWrapper::treatDescriptionChanging()
{
    mControlWidgetCopy->treatDescriptionChanging();
}

void EventControlWidgetWrapper::treatEventDeleting(QObject* event)
{
    mControlWidgetCopy->treatEventDeleting(event);
}

void EventControlWidgetWrapper::treatLabelDeviationChanging()
{
    mControlWidgetCopy->treatLabelDeviationChanging();
}

//void EventControlWidgetWrapper::treatBrushColorChanging()
//{
//    controlWidget->treatBrushColorChanging();
//}

//void EventControlWidgetWrapper::showBrushColorDialog()
//{
//    controlWidget->showBrushColorDialog();
//}

void EventControlWidgetWrapper::treatBrushStyleChanging()
{
    mControlWidgetCopy->treatBrushStyleChanging();
}

void EventControlWidgetWrapper::treatBrushBasisChanging()
{
    mControlWidgetCopy->treatBrushBasisChanging();
}



