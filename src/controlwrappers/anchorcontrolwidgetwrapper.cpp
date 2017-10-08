#include "anchorcontrolwidgetwrapper.h"
#include "assert.h"


AnchorControlWidgetWrapper::AnchorControlWidgetWrapper(QWidget *parent)
    :ControlWidgetWrapper(new AnchorControlWidget(), parent), mControlWidgetCopy(NULL)
{
    mControlWidgetCopy = dynamic_cast<AnchorControlWidget*>(mControlWidget);
    assert(mControlWidgetCopy != NULL);
    setHeaderText(tr("Anchor properties"));

    connect(mControlWidgetCopy, SIGNAL(widgetIsHiding()),
            this, SLOT(treatHidingOfInternalWidget()));
    connect(mControlWidgetCopy, SIGNAL(arrowedTextCreationRequest(double)),
            this, SIGNAL(arrowedTextCreationRequest(double)));


}

void AnchorControlWidgetWrapper::setAnchor(Anchor *anchor)
{
    mControlWidgetCopy->setAnchor(anchor);
}

void AnchorControlWidgetWrapper::treatNameChanging()
{
    mControlWidgetCopy->treatNameChanging();
}

void AnchorControlWidgetWrapper::treatTimeChanging()
{
    mControlWidgetCopy->treatTimeChanging();
}

void AnchorControlWidgetWrapper::treatAnchorDeleting(QObject *anchor)
{
    mControlWidgetCopy->treatAnchorDeleting(anchor);
}



